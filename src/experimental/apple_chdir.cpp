#ifdef __APPLE__

#include "CoreFoundation/CoreFoundation.h"

#include <fstream>

void appleChdir() {
    std::ofstream f("/Users/aeubanks/mapgen.log");
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX)) {
        puts("CFURLGetFileSystemRepresentation fail\n");
    }
    CFRelease(resourcesURL);

    chdir(path);
    f << "cur path: " << path << '\n';
    puts("cur path:");
    puts(path);
}
#endif
