import cocotb
from cocotb.clock import Clock
from cocotb.triggers import RisingEdge

async def reset(dut):
    dut.rst.setimmediatevalue(0)
    await RisingEdge(dut.clk)
    await RisingEdge(dut.clk)
    dut.rst.value = 1
    await RisingEdge(dut.clk)
    await RisingEdge(dut.clk)
    dut.rst.value = 0
    await RisingEdge(dut.clk)
    await RisingEdge(dut.clk)

@cocotb.test()
async def test_wrapper(dut):
    dut.test_in_val.setimmediatevalue(0)
    dut.test_in_len.setimmediatevalue(0)
    dut.test_out_rdy.setimmediatevalue(0)

    cocotb.start_soon(Clock(dut.clk, tb.CLOCK_CYCLE_TIME, units="ns").start())

    await reset(dut)

    for i in range(0, 10):
        await RisingEdge(dut.clk)
