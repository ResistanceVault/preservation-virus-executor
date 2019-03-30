struct File
{
   char *host; // hostname (www.domain.com)
   char *path; // path to file (/user/files/file.lha)
   char *name; // filename (file.lha)
   char *data; // file data buffer (file.lha)
   char *new;  // What is new in this version
   char *info; // The Short info header
   long size;  // size of file (size of file.lha)
   char *version;    // version string (must be %d.%d.%d...)
   char *local_path; // local path on your harddrive
   char *program_name;  // Name of program and directory
   int s;      // socket
   int port;   // port (always 80 for now...)
};
void upd_Updater(struct File *file, char *this_ver, int debug);

