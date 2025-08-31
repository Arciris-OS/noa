#ifndef _SWITCH_ROOT
#define  _SWITCH_ROOT

int delete_contents(const char* directory);
int mount_with_loop(const char *image_file, const char *mountpoint, const char *fstype);
int switch_root(const char* newroot, const char* init);

#endif /* _SWITCH_ROOT */



