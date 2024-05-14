module test(
     input clk
    ,input rst

    ,input          test_in_val
    ,input  [7:0]   test_in_len
    ,output         test_in_rdy


    ,output         test_out_val
    ,input          test_out_rdy
);

    typedef enum logic {
        READY = 1'b0,
        COUNT = 1'b1,
        UND = 'X
    } state_e;

    state_e state_reg;
    state_e state_next;

    logic   [7:0]   test_len_reg;
    logic   [7:0]   test_len_next;

    logic   [7:0]   count_reg;
    logic   [7:0]   count_next;

    logic           store_len;
    logic           incr_count;

    logic           last_in;

    always_ff @(posedge clk) begin
        if (rst) begin
            state_reg <= READY;
        end
        else begin
            state_reg <= state_next;
            test_len_reg <= test_len_next;
            count_reg <= count_next;
        end
    end

    assign count_next = store_len
                        ? '0
                        : incr_count
                            ? count_reg + 1'b1
                            : count_reg;

    assign test_len_next = store_len
                        ? test_in_len
                        : test_len_reg;

    assign last_in = count_next == test_len_next;

    assign test_out_val = 1'b1;
    assign test_in_rdy = 1'b1;

    always_comb begin
        store_len = 1'b0;
        incr_count = 1'b0;

        state_next = state_reg;
        case (state_reg)
            READY: begin
                store_len = 1'b1;
                if (test_in_val & test_out_rdy) begin
                    state_next = COUNT;
                end
            end
            COUNT: begin
                if (test_in_val & test_out_rdy) begin
                    incr_count = 1'b1;
                    if (last_in) begin
                        state_next = READY;
                    end
                end
            end
        endcase
    end

endmodule
