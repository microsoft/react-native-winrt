
#include "nodewinrt/base.h"

#pragma comment(lib, "runtimeobject.lib")
#pragma comment(lib, "rpcrt4.lib")

NODE_MODULE_INIT(/*exports, module, context*/)
{
    nodewinrt::register_exports(context, exports);
}
