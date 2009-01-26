inline void initlocale ()
{
#ifdef ___DEBUG___
  int i;
  for (i = 0; i < 1000; i++)
    malloc_addr[i] = 0;
#endif /* ___DEBUG___ */
  sbrk (100000);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
}
