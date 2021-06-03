// Use something more generous than 16.  Maybe even 4096 or 64k and allocate buffers
#define CMP_N 256

// Return value:
// 0: files compare equal in content and length, fp1 size saved as offset
// 1: files differ, fp1 longer, fp2 size saved as offset
// 2: files differ, fp2 longer, fp1 size saved as offset
// 3: files differ at offset
// -1: fp1 trouble reading.  Unspecified data in offset
// -2: fp2 trouble reading.  Unspecified data in offset
int compare_two_binary_files_alternate(FILE *fp1, FILE *fp2, uintmax_t *offset) {
  char tmp1[CMP_N], tmp2[CMP_N];
  size_t n1, n2;

  rewind(fp1);  // start at beginning and clear error
  rewind(fp2);
  *offset = 0;
  do {
    n1 = fread(tmp1, sizeof *tmp1, sizeof tmp1 / sizeof *tmp1, fp1);
    if (n1 == 0 && ferror(fp1)) {
      return -1;
    }
    n2 = fread(tmp2, sizeof *tmp2, sizeof tmp2 / sizeof *tmp2, fp2);
    if (n2 == 0 && ferror(fp2)) {
      return -2;
    }
    size_t n_min = n1 < n2 ? n1 : n2;
    if (memcmp(tmp1, tmp2, n_min)) {        // Quickly find if file contents differ ...
      for (size_t i = 0; i < n_min; i++) {  // Slowly find where they differ
        if (tmp1[i] != tmp2[i]) {
          *offset += i;
          return 3;
        }
      }
    }
    *offset += n_min;
    if (n1 > n_min) {
      return 1;
    }
    if (n2 > n_min) {
      return 2;
    }
  } while (n1);
  return 0;
}
