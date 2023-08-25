const Dnssd = Windows.Networking.ServiceDiscovery.Dnssd;
const DnssdServiceWatcher = Dnssd.DnssdServiceWatcher;

/**
 * Encapsulates an instance of a service that uses DNS Service Discovery (DNS-SD).
 * This interface is based on Windows.Networking.ServiceDiscovery.Dnssd.DnssdServiceInstance.
 * 
 * https://learn.microsoft.com/en-us/uwp/api/windows.networking.servicediscovery.dnssd.dnssdserviceinstance?view=winrt-22621
 */
export interface IDnssdServiceInstance {
    /** Host name of discovered service. */
    hostName: string;

    /** Service type portion of DNS-SD service instance name. (e.g. "_ipp._tcp" in "myservice._ipp._tcp.local") */
    serviceName: string;

    /** Instance portion of DNS-SD service instance name.(e.g. "myservice" in "myservice._ipp._tcp.local") */
    instanceName: string;

    /**
     * An array of IP addresses associated with discovered service.
     * Note: 
     * - 0th item is IPv4 address.
     * - 1st item is IPv6 address.
     */
    ipAddresses: string[];

    /** IPv4 address of discovered service. Corresponds to 0th item of `ipAddresses` array. */
    ipv4Address: string;

    /** IPv6 address of discovered service. Corresponds to 1st item of `ipAddresses` array. */
    ipv6Address: string;

    /** Port number on which service is listening. */
    portNumber: number;

    /** Text data associated with the service instance. Each string is typically a key-value pair, separated by "=". */
    textAttributes: string;
}

/**
 * Helper class for looking up devices using the DNS-SD protocol.
 * The class is implemented as a singleton, ensuring only one instance is created.
 * 
 * @remarks
 * This helper focuses on devices that adhere to specific query parameters defined internally. If needed, this can easily be made more generic.
 */
export class DnssdLookupHelper {
    private static instance: DnssdLookupHelper;

    // Used to construct AQS query
    // For specifics, refer https://learn.microsoft.com/en-us/windows/uwp/devices-sensors/enumerate-devices-over-a-network.
    private static readonly PROTOCOL_GUID = "{4526e8c1-8aac-4153-9b16-55e86ada0e54}"; // Protocol type for DNS-SD.
    private static readonly DOMAIN = "local";
    private static readonly SERVICE_NAME = "_ssh._tcp";
    // private static readonly SERVICE_NAME = "_ipp._tcp";

    // Used to construct an iterable list of properties to look for in the devices that are discovered.
    private static readonly HOSTNAME_PROPERTY = "System.Devices.Dnssd.HostName";
    private static readonly SERVICENAME_PROPERTY = "System.Devices.Dnssd.ServiceName";
    private static readonly INSTANCENAME_PROPERTY = "System.Devices.Dnssd.InstanceName";
    private static readonly IPADDRESS_PROPERTY = "System.Devices.IpAddress";
    private static readonly PORTNUMBER_PROPERTY = "System.Devices.Dnssd.PortNumber";
    private static readonly TEXTATTRIBUTES_PROPERTY = "System.Devices.Dnssd.TextAttributes";

    // TO DO: Support changing propertyKeys from consumer-side if needed.
    private propertyKeys = [
        DnssdLookupHelper.HOSTNAME_PROPERTY,
        DnssdLookupHelper.SERVICENAME_PROPERTY,
        DnssdLookupHelper.INSTANCENAME_PROPERTY,
        DnssdLookupHelper.IPADDRESS_PROPERTY,
        DnssdLookupHelper.PORTNUMBER_PROPERTY,
        DnssdLookupHelper.TEXTATTRIBUTES_PROPERTY
    ];

    private constructor() { } // Private constructor ensures no external instantiation

    /**
     * Gets the singleton instance of `DnssdLookupHelper`.
     * If no instance exists, it initializes a new one.
     * @example
     * const finder = DnssdLookupHelper.getInstance();
     * @returns The singleton instance of the `DnssdLookupHelper` class.
     */
    public static getInstance(): DnssdLookupHelper {
        if (!DnssdLookupHelper.instance) {
            DnssdLookupHelper.instance = new DnssdLookupHelper();
        }
        return DnssdLookupHelper.instance;
    }

    /**
     * Enumerates IDnssdServiceInstance implementing objects matching the query parameters.
     * @example
     * const finder = DnssdLookupHelper.getInstance();
     * let devices = await finder.findAllDevicesAsync();
     * @returns A Promise that resolves to an array of objects implementing IDnssdServiceInstance.
     */
    public async findAllDevicesAsync(): Promise<IDnssdServiceInstance[]> {
        const services: IDnssdServiceInstance[] = [];
    
        try {
            const query = `System.Devices.AepService.ProtocolId:=${DnssdLookupHelper.PROTOCOL_GUID} AND System.Devices.Dnssd.Domain:=${DnssdLookupHelper.DOMAIN} AND System.Devices.Dnssd.ServiceName:=${DnssdLookupHelper.SERVICE_NAME}`;
    
            // NOTE: Casting `propertyKeys` to `unknown` first, and then to `Windows.Foundation.Collections.IIterable<string>`
            // is a workaround to bypass type-related errors thrown by VS Code and in the runtime.
            const devices = await Windows.Devices.Enumeration.DeviceInformation.findAllAsync(
                query,
                (this.propertyKeys as unknown as Windows.Foundation.Collections.IIterable<string>),
                Windows.Devices.Enumeration.DeviceInformationKind.associationEndpointService
            );
    
            for (let i = 0; i < devices.size; i++) {
                const device = devices.getAt(i);
                const properties = device.properties;
                const serviceInfo: Partial<IDnssdServiceInstance> = {};
    
                this.propertyKeys.forEach((key: string) => {
                    if (properties.hasKey(key)) {
                        const value = properties.lookup(key);
                        switch (key) {
                            case DnssdLookupHelper.HOSTNAME_PROPERTY:
                                serviceInfo.hostName = value;
                                break;
                            case DnssdLookupHelper.SERVICENAME_PROPERTY:
                                serviceInfo.serviceName = value;
                                break;
                            case DnssdLookupHelper.INSTANCENAME_PROPERTY:
                                serviceInfo.instanceName = value;
                                break;
                            case DnssdLookupHelper.IPADDRESS_PROPERTY:
                            let ipAddressesArray = value as string[];
                            serviceInfo.ipAddresses = ipAddressesArray;
                            
                            // Handles cases where:
                            // 1. Both ipv4 and ipv6 addresses are present
                            // 2. Only an ipv6 address is present
                            if (ipAddressesArray.length > 0) {
                                // Regex to check if an IP address is IPv4
                                const ipv4Pattern = /^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.?\b){4}$/;

                                if (ipv4Pattern.test(ipAddressesArray[0])) {
                                    serviceInfo.ipv4Address = ipAddressesArray[0];
                                } else {
                                    serviceInfo.ipv6Address = ipAddressesArray[0];
                                }
                            }

                            // Handles cases where both ipv4 and ipv6 addresses are present
                            if (ipAddressesArray.length > 1) {
                                // If two addresses are present, assume the second is always IPv6
                                serviceInfo.ipv6Address = ipAddressesArray[1];
                            }
                            break;
                            case DnssdLookupHelper.PORTNUMBER_PROPERTY:
                                serviceInfo.portNumber = value;
                                break;
                            case DnssdLookupHelper.TEXTATTRIBUTES_PROPERTY:
                                serviceInfo.textAttributes = value;
                                break;
                        }
                    }
                });

                services.push(serviceInfo as IDnssdServiceInstance);
            }
            console.log(`Found ${services.length} service(s)`);
        } catch (error) {
            console.error("Error discovering devices:", error);
        }
    
        return services;
    }
}