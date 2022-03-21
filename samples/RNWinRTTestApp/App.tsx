// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

import React from "react";
import {
    Image,
    SafeAreaView,
    StyleSheet,
    ScrollView,
    View,
    Text,
    StatusBar,
    Pressable,
    Linking,
} from "react-native";

import {
    Colors,
    Header,
} from "react-native/Libraries/NewAppScreen";

import { showNotification } from './Notifications';

async function updateJumpListAsync(): Promise<void> {
    try {
        const StartScreenApi = Windows.UI.StartScreen;
        const jumplist = await StartScreenApi.JumpList.loadCurrentAsync();

        jumplist.systemGroupKind = StartScreenApi.JumpListSystemGroupKind.recent;

        const items = jumplist.items;
        items.clear();

        const customCategoryName = "Custom Category";
        {
            const date = new Date();
            const customCategoryItem = StartScreenApi.JumpListItem.createWithArguments("custom1", `Updated at ${date.toTimeString()} on ${date.toDateString()}`);
            customCategoryItem.groupName = customCategoryName;
            customCategoryItem.logo = new Windows.Foundation.Uri("ms-appx:///Assets/Square44x44Logo.png");
            items.append(customCategoryItem);
        }

        {
            const customCategoryItem = StartScreenApi.JumpListItem.createWithArguments("custom2", `Proof JumpList is still updating!`);
            customCategoryItem.description = "and that IAsyncOperation and IVector are reflected correctly!";
            customCategoryItem.groupName = customCategoryName;
            customCategoryItem.logo = new Windows.Foundation.Uri("ms-appx:///Assets/code.png");
            items.append(customCategoryItem);
        }

        {
            const customCategoryItem = StartScreenApi.JumpListItem.createWithArguments("custom3", `We should handle app launcher args too`);
            customCategoryItem.description = "to prove that JumpList will work fully end-to-end beyond WinRT projections.";
            customCategoryItem.groupName = customCategoryName;
            customCategoryItem.logo = new Windows.Foundation.Uri("ms-appx:///Assets/video.png");
            items.append(customCategoryItem);
        }

        {
            const taskItem = StartScreenApi.JumpListItem.createWithArguments("task1", "Task 1");
            taskItem.description = "Still visible when recent is disabled by policy.";
            taskItem.logo = new Windows.Foundation.Uri("ms-appx:///Assets/folder.png");
            items.append(taskItem);
        }

        {
            const taskItem = StartScreenApi.JumpListItem.createWithArguments("task2", "Task 2");
            taskItem.description = "Also visible when user disables the setting for recent items.";
            taskItem.logo = new Windows.Foundation.Uri("ms-appx:///Assets/exe.png");
            items.append(taskItem);
        }

        await jumplist.saveAsync();
    } catch (e) {
        console.error(`Failed to update JumpList due to ${e}`);
    }
}

// PERF: Base64 is hugely wasteful when used for representing images. Strongly consider using it for small images only,
//       and find other means for other images. Inefficiency comes from Base64 itself, 16-bit string encoding (may be
//       used by the JS engine internally), and copies of the string stay in memory (or shadow DOM copies) even after the
//       native code has decoded it.
export async function createBase64DataUriFromRandomAccessStreamAsync(stream: Windows.Storage.Streams.IRandomAccessStream, contentTypeHint?: string): Promise<string> {
    const dataSize = stream.size;
    if (dataSize > 0) {
        let contentType = (stream as Windows.Storage.Streams.IRandomAccessStreamWithContentType).contentType;
        if (!contentType) {
            contentType = contentTypeHint || "";
        }

        const buffer = await stream.readAsync(new Windows.Storage.Streams.Buffer(dataSize), dataSize, Windows.Storage.Streams.InputStreamOptions.none);
        const base64Data = Windows.Security.Cryptography.CryptographicBuffer.encodeToBase64String(buffer);
        return base64Data ? `data:${contentType};base64,${base64Data}` : "";
    }
    return "";
}

async function getPictureThumbnailAsync(): Promise<string> {
    try {
        const StorageApi = Windows.Storage;

        const library = StorageApi.KnownFolders.picturesLibrary;
        let files = await library.getFilesAsync();

        let file: Windows.Storage.StorageFile | null = null;
        if (files.size > 0) {
            file = files.getAt(0);
        } else {
            const folders = await library.getFoldersAsync();
            const numFolders = folders.size;
            for (let i = 0; i < numFolders; ++i) {
                files = await folders.getAt(i).getFilesAsync();
                if (files.size > 0) {
                    file = files.getAt(0);
                }
            }
        }

        if (file) {
            const stream = await files.getAt(0).openReadAsync();
            return await createBase64DataUriFromRandomAccessStreamAsync(stream);
        }
    } catch (e) {
        console.error(e);
    }
    return "";
}

interface AsyncImageProps {
    imageUriPromise?: Promise<string>;
}

function AsyncImage(props: AsyncImageProps) {
    const [imageUri, setImageUri] = React.useState("");

    if (props.imageUriPromise && !imageUri) {
        props.imageUriPromise.then((result: string) => {
            setImageUri(result);
        });
    }

    // TODO: RN cannot compute the width/height for data: or web URIs but it seems like RNW is failing to honor
    //       resizeMode/resizeMethod as it is still stretching the image rather than scaling to fit.
    return imageUri ? (
        <Image
            source={{ uri: imageUri }}
            resizeMode="contain"
            resizeMethod="resize"
            style={{ width: 400, height: 300 }}
        />
    ) : (
        <Text style={styles.sectionDescription}>
            {`Loads an image here from your Pictures library folder if one is present`}
        </Text>
    );
}

const App = () => {
    updateJumpListAsync();
    // NOTE: The Id used is the hash of the string 'SampleProvider'. This Guid is 'eff1e128-4903-5093-096a-bdc29b38456f'
    const loggingChannel = new Windows.Foundation.Diagnostics.LoggingChannel("SampleProvider", null);
    const imageUriPromise = getPictureThumbnailAsync();

    return (
        <>
            <StatusBar barStyle="dark-content" />
            <SafeAreaView style={styles.root}>
                <ScrollView
                    contentInsetAdjustmentBehavior="automatic"
                    style={styles.scrollView}>
                    <Header />
                    <View style={styles.body}>
                        <View style={styles.sectionContainer}>
                            <Text style={styles.sectionTitle}>Windows.UI.StartScreen.JumpList Example</Text>
                            <Text style={styles.sectionDescription}>
                                Right click the app in Start or Taskbar to see the populated JumpList.
                            </Text>
                        </View>
                    </View>
                    <View style={styles.body}>
                        <View style={styles.sectionContainer}>
                            <Text style={styles.sectionTitle}>Windows.Foundation.Diagnostics.LoggingChannel Example</Text>
                            <Text style={styles.sectionDescription} selectable={true}>
                                Click the following buttons to log events. These
                                events can be seen in different ETW logging tools such as{' '}
                                <Text style={{color: 'blue', textDecorationLine: 'underline'}} accessibilityRole='link' onPress={() => Linking.openURL('https://docs.microsoft.com/windows/uwp/debug-test-perf/device-portal#event-tracing-for-windows-etw-logging')}>Device Portal</Text>
                                {' '}or xperf. To view
                                these events, use the provider GUID{' '}
                                <Text style={{fontFamily: 'Consolas', backgroundColor: 'whitesmoke'}}>eff1e128-4903-5093-096a-bdc29b38456f</Text>
                            </Text>
                            <View style={{ paddingTop: 10, flexDirection: 'row', justifyContent: 'center', flexWrap: 'wrap', alignItems: 'center' }}>
                                <Pressable style={[{marginHorizontal: 10}, styles.sectionDescriptionButton]} onPress={() => {
                                    loggingChannel.logMessage("This string is being logged from an RNW application!");
                                }}>
                                    <Text style={styles.sectionDescriptionButtonText}>Log String</Text>
                                </Pressable>
                                <Pressable style={[{marginHorizontal: 10}, styles.sectionDescriptionButton]} onPress={() => {
                                    loggingChannel.logValuePair("This value is being logged from an RNW application!", 42);
                                }}>
                                    <Text style={styles.sectionDescriptionButtonText}>Log Value Pair</Text>
                                </Pressable>
                                <Pressable style={[{marginHorizontal: 10}, styles.sectionDescriptionButton]} onPress={() => {
                                    var fields = new Windows.Foundation.Diagnostics.LoggingFields();
                                    fields.addString("string", "Hello, from RNW!");
                                    fields.addInt32("int32", 42);
                                    fields.addDateTime("date", new Date());
                                    loggingChannel.logEvent("This structure is being logged from an RNW application!", fields);
                                }}>
                                    <Text style={styles.sectionDescriptionButtonText}>Log With Fields</Text>
                                </Pressable>
                            </View>
                        </View>
                    </View>
                    <View style={styles.body}>
                        <View style={styles.sectionContainer}>
                            <Text style={styles.sectionTitle}>Windows.UI.Notifications Example</Text>
                            <View style={{ flexDirection: 'row', flexWrap: 'wrap', alignItems: 'center' }}>
                                <Text style={[{ paddingRight: 10 }, styles.sectionDescription]}>Click the button to show a notification: </Text>
                                <Pressable style={styles.sectionDescriptionButton} onPress={() => {
                                    showNotification({
                                        template: Windows.UI.Notifications.ToastTemplateType.toastImageAndText01,
                                        // The template schema can be found at https://docs.microsoft.com/previous-versions/windows/apps/hh761494(v=win.10)
                                        text: "hello world",
                                        image: {
                                            src: "https://microsoft.github.io/react-native-windows/img/header_logo.svg",
                                            alt: "React logo",
                                        }
                                    });
                                }}>
                                    <Text style={styles.sectionDescriptionButtonText}>Press</Text>
                                </Pressable>
                            </View>
                        </View>
                    </View>
                    <View style={styles.body}>
                        <View style={styles.sectionContainer}>
                            <Text style={styles.sectionTitle}>Windows.Storage API + picturesLibrary capability Example</Text>
                            <AsyncImage imageUriPromise={imageUriPromise} />
                        </View>
                    </View>
                </ScrollView>
            </SafeAreaView>
        </>
    );
};

const styles = StyleSheet.create({
    scrollView: {
        backgroundColor: Colors.white,
    },
    engine: {
        position: "absolute",
        right: 0,
    },
    body: {
        backgroundColor: Colors.white,
    },
    root: {
        backgroundColor: Colors.white,
        flex: 1,
    },
    sectionContainer: {
        marginTop: 32,
        paddingHorizontal: 24,
    },
    sectionTitle: {
        fontSize: 24,
        fontWeight: "600",
        color: Colors.black,
    },
    sectionDescription: {
        marginTop: 8,
        fontSize: 18,
        fontWeight: "400",
        color: Colors.dark,
    },
    sectionDescriptionButton: {
        alignItems: 'center',
        justifyContent: 'center',
        paddingVertical: 4,
        paddingHorizontal: 24,
        borderRadius: 1,
        backgroundColor: Colors.light,
    },
    sectionDescriptionButtonText: {
        fontSize: 18,
        fontWeight: "400",
        color: Colors.dark,
    },
    highlight: {
        fontWeight: "700",
    },
    footer: {
        color: Colors.dark,
        fontSize: 12,
        fontWeight: "600",
        padding: 4,
        paddingRight: 12,
        textAlign: "right",
    },
});

export default App;
