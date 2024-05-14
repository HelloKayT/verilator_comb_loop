// Include common routines
#include <verilated.h>

// Include model header, generated from Verilating "top.v"
#include "Vmux_sim_top.h"

static void time_step(const std::unique_ptr<VerilatedContext> &contextp,
                    const std::unique_ptr<Vmux_sim_top> &top) {
    contextp->timeInc(1);  // 1 timeprecision period passes...
    top->eval();
}

static void half_clock_cycle(const std::unique_ptr<VerilatedContext> &contextp,
                            const std::unique_ptr<Vmultiplier_top> &top) {
        top->clk = !top->clk;
            for (int i = 0; i < CLOCK_HALF_CYCLE_NS; i++) {
                        time_step(contextp, top);
                            }
}

static void clock_cycle(const std::unique_ptr<VerilatedContext> &contextp,
                            const std::unique_ptr<Vmultiplier_top> &top) {
        half_clock_cycle(contextp, top);
            half_clock_cycle(contextp, top);
}

static void init_context(const std::unique_ptr<VerilatedContext> &contextp,
                         int argc,
                         char ** argv) {
    // Set debug level, 0 is off, 9 is highest presently used
    // May be overridden by commandArgs argument parsing
    contextp->debug(0);

    // Randomization reset policy
    // May be overridden by commandArgs argument parsing
    contextp->randReset(2);

    // Verilator must compute traced signals
    contextp->traceEverOn(true);

    // Pass arguments so Verilated code can see them, e.g. $value$plusargs
    // This needs to be called before you create any model
    contextp->commandArgs(argc, argv);
}


int main(int argc, char** argv, char** env) {
    // Prevent unused variable warnings
    if (false && argc && argv && env) {}
    
    // Create logs/ directory in case we have traces to put under it
    Verilated::mkdir("logs");
    
    // Using unique_ptr is similar to
    // "VerilatedContext* contextp = new VerilatedContext" then deleting at end.
    const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};
    
    init_context(contextp, argc, argv);

    const std::unique_ptr<Vmux_sim_top> top{new Vmux_sim_top{contextp.get(), "TOP"}};

    top->clk = 0;
    top->rst = 0;

    top->test_in_val = 0;
    top->test_in_len = 0;

    top->test_out_rdy = 0;

    clock_cycle(contextp, top);
    clock_cycle(contextp, top);
    clock_cycle(contextp, top);
    clock_cycle(contextp, top);
    clock_cycle(contextp, top);
    clock_cycle(contextp, top);
    return 0;
}
