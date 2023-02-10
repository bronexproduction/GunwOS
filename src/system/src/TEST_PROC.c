extern void k_proc_spawn(void (*func)());
extern int c_trm_puts(const char * const s);

static void proc1() {
    while (1) {
        c_trm_puts("1");
    }
}

static void proc2() {
    while (1) {
        c_trm_puts("2");
    }
}

void TEST_PROC_SPAWN() {
    k_proc_spawn(proc1);
    k_proc_spawn(proc2);
}
