/*
 * WARNING:
 * Do not modify this file, otherwise the library may become incompatible.
 */

#define KiB (1024lu)
#define MiB (1024 * KiB)
#define GiB (1024 * MiB)
#define PAGE (4 * KiB)
#define LARGE_PAGE (2 * MiB)
#define total_size (1 * GiB)

/*
 * Performs memory allocation
 * srno = Last 5 digits of your IISc serial no.
 * return value = address of the allocated memory
 */
void *work_init(unsigned int srno);

/*
 * Start executing the workload
 */
int work_run();
