/* -*- c++ -*- */
/* 
 * Copyright 2015 Free Software Foundation, Inc.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_DTV_DVBT2_INTERLEAVER_BB_IMPL_H
#define INCLUDED_DTV_DVBT2_INTERLEAVER_BB_IMPL_H

#include <gnuradio/dtv/dvbt2_interleaver_bb.h>
#include "dvb/dvb_defines.h"

struct FuncHandlerDataStruct {
  FuncHandlerDataStruct(  
                          const unsigned char *d_in,
                          unsigned char *d_out,
                          const int arg_idx,
                          const int arg_num_core,
                          const int arg_val_packed_items,
                          const int d_rows,
                          const int d_local_nbch,
                          const int d_local_q_val) :
        in(d_in),
        out(d_out),
        idx(arg_idx),
        num_core(arg_num_core),
        val_packed_items(arg_val_packed_items),
        rows(d_rows),
        local_nbch(d_local_nbch),
        local_q_val(d_local_q_val) {}
  const unsigned char *in;
  unsigned char *out;
  const int idx;
  const int num_core;
  const int val_packed_items;
  const int rows;
  const int local_nbch;
  const int local_q_val;
};

namespace gr {
  namespace dtv {

    class dvbt2_interleaver_bb_impl : public dvbt2_interleaver_bb
    {
     private:
      int frame_size;
      int signal_constellation;
      int code_rate;
      int nbch;
      int q_val;
      int mod;
      int packed_items;

      const static int twist16n[8];
      const static int twist64n[12];
      const static int twist256n[16];

      const static int twist16s[8];
      const static int twist64s[12];
      const static int twist256s[8];

      const static int mux16[8];
      const static int mux64[12];
      const static int mux256[16];

      const static int mux16_35[8];
      const static int mux16_13[8];
      const static int mux16_25[8];
      const static int mux64_35[12];
      const static int mux64_13[12];
      const static int mux64_25[12];
      const static int mux256_35[16];
      const static int mux256_23[16];

      const static int mux256s[8];
      const static int mux256s_13[8];
      const static int mux256s_25[8];

/************** Function handler **************/
      static void func_handler_mod_qpsk_c13_c25(FuncHandlerDataStruct);
      static void func_handler_mod_qpsk_rest(FuncHandlerDataStruct);
      static void func_handler_mod_16qam(FuncHandlerDataStruct, const int, const int*, const int*);
      static void func_handler_mod_64qam(FuncHandlerDataStruct, const int, const int*, const int*);
      static void func_handler_mod_256qam_frame_size_norm(FuncHandlerDataStruct, const int, const int*, const int*);
      static void func_handler_mod_256qam_rest(FuncHandlerDataStruct, const int, const int*, const int*);
/********************************************************/

     public:
      dvbt2_interleaver_bb_impl(dvb_framesize_t framesize, dvb_code_rate_t rate, dvb_constellation_t constellation);
      ~dvbt2_interleaver_bb_impl();

      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items);
    };
  } // namespace dtv
} // namespace gr

#endif /* INCLUDED_DTV_DVBT2_INTERLEAVER_BB_IMPL_H */

