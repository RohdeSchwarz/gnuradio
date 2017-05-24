/* -*- c++ -*- */
/* 
 * Copyright 2015,2016 Free Software Foundation, Inc.
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

#ifndef INCLUDED_DTV_DVB_BCH_BB_IMPL_H
#define INCLUDED_DTV_DVB_BCH_BB_IMPL_H

#include <gnuradio/dtv/dvb_bch_bb.h>
#include "dvb_defines.h"

namespace gr {
  namespace dtv {

    class dvb_bch_bb_impl : public dvb_bch_bb
    {
     private:
      unsigned int kbch;
      unsigned int nbch;
      unsigned int bch_code;
      //unsigned int m_poly_n_8[4];
      //unsigned int m_poly_n_10[5];
      //unsigned int m_poly_n_12[6];
      //unsigned int m_poly_s_12[6];
      //unsigned int m_poly_m_12[6];
      uint64_t m_poly64_n_8[2];
      uint64_t m_poly64_n_10[3];
      uint64_t m_poly64_n_12[3];
      uint64_t m_poly64_s_12[3];
      uint64_t m_poly64_m_12[3];
	  union{
		unsigned char b[256][8];
		uint64_t lw[256];
	  }output_lookup_table;
      int poly_mult(const int*, int, const int*, int, int*);
      void poly_pack(const int*, unsigned int*, int);
      void poly_pack64(const int*, uint64_t*, int);
      void poly_reverse(int*, int*, int);
      void output_lut_build(void);
      //inline void reg_4_shift(unsigned int*);
      //inline void reg_5_shift(unsigned int*);
      //inline void reg_6_shift(unsigned int*);
	  inline void reg_128b_shift(uint64_t*);
	  inline void reg_160b_shift(uint64_t*);
	  inline void reg_192b_shift(uint64_t*);
      inline void reg_128b_shift8(uint64_t*);
	  inline void reg_160b_shift8(uint64_t*);
	  inline void reg_192b_shift8(uint64_t*);
      void bch_poly_build_tables(void);

     public:
      dvb_bch_bb_impl(dvb_standard_t standard, dvb_framesize_t framesize, dvb_code_rate_t rate);
      ~dvb_bch_bb_impl();

      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items);
    };

  } // namespace dtv
} // namespace gr

#endif /* INCLUDED_DTV_DVB_BCH_BB_IMPL_H */

