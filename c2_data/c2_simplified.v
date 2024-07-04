// Benchmark "top" written by ABC on Fri Aug 18 16:47:21 2023

module top ( 
    x0, x1, y0, y1, z,
    u0, u1, w0, w1  );
  input  x0, x1, y0, y1, z;
  output u0, u1, w0, w1;
  wire new_n11_, new_n12_, new_n13_, new_n14_, new_n16_, new_n17_, new_n18_,
    new_n20_;
  assign u0 = x0 & y0;
  assign new_n11_ = x1 & y0;
  assign new_n12_ = x0 & y1;
  assign new_n13_ = new_n11_ & ~new_n12_;
  assign new_n14_ = ~new_n11_ & new_n12_;
  assign u1 = new_n13_ | new_n14_;
  assign new_n16_ = y0 & z;
  assign new_n17_ = ~y1 & ~new_n16_;
  assign new_n18_ = y1 & new_n16_;
  assign w0 = ~new_n17_ & ~new_n18_;
  assign new_n20_ = ~y0 & ~z;
  assign w1 = ~new_n16_ & ~new_n20_;
endmodule


