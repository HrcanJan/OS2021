uint64 nproc(void);
uint64 freemem(void);

struct sysinfo {
  uint64 freemem;   // amount of free memory (bytes)
  uint64 nproc;     // number of process
};
