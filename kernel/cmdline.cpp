
extern const char* s_cmdline;

static char match_name(const char* name, const char*& sp)
{
  while (*name && *sp && *sp != '=') {
    if (*name++ != *sp++)
      return false;
  }
  sp++;
  return true;
}

static bool next_name(const char*& sp)
{
  while (*sp != ' ') {
    if (*sp++ == 0)
      return false;
  }
  sp++;
  return true;
}

static void copy_value(const char*& sp, char* buffer)
{
  while (*sp && *sp != ' ') {
    *buffer++ = *sp++;
  }
  *buffer = 0;
}

bool cmdline_get(const char* name, char* buffer)
{
  const char* sp = s_cmdline;
  if (*name == 0)
    return false;

  do {
    if (match_name(name, sp)) {
      copy_value(sp, buffer);
      return true;
    }
  } while (next_name(sp));
  return false;
}
