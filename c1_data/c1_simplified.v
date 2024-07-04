// Benchmark "top" written by ABC on Fri Aug 18 16:47:20 2023

module top ( 
    a0, a1, b1, b0, c,
    h0, h1, m0, m1  );
  input  a0, a1, b1, b0, c;
  output h0, h1, m0, m1;
  wire new_n11_, new_n12_, new_n13_, new_n14_, new_n16_, new_n17_, new_n18_,
    new_n20_;
  assign h0 = a0 & b0;
  assign new_n11_ = a1 & b0;
  assign new_n12_ = a0 & b1;
  assign new_n13_ = new_n11_ & ~new_n12_;
  assign new_n14_ = ~new_n11_ & new_n12_;
  assign h1 = new_n13_ | new_n14_;
  assign new_n16_ = b0 & c;
  assign new_n17_ = ~b1 & ~new_n16_;
  assign new_n18_ = b1 & new_n16_;
  assign m0 = ~new_n17_ & ~new_n18_;
  assign new_n20_ = ~b0 & ~c;
  assign m1 = ~new_n16_ & ~new_n20_;
endmodule


