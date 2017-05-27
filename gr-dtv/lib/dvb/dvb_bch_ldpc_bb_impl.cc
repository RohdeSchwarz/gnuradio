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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <immintrin.h>
#include <iostream>

#include <gnuradio/io_signature.h>
#include "dvb_bch_ldpc_bb_impl.h"
#include "dvb_bch_ldpc_bb_impl.h"
#include <boost/math/common_factor.hpp>


namespace gr {
  namespace dtv {

    dvb_bch_ldpc_bb::sptr
    dvb_bch_ldpc_bb::make(dvb_standard_t standard, dvb_framesize_t framesize, dvb_code_rate_t rate, dvb_constellation_t constellation)
    {
      return gnuradio::get_initial_sptr
        (new dvb_bch_ldpc_bb_impl(standard, framesize, rate, constellation));
    }

    /*
     * The private constructor
     */
    dvb_bch_ldpc_bb_impl::dvb_bch_ldpc_bb_impl(dvb_standard_t standard, dvb_framesize_t framesize, dvb_code_rate_t rate, dvb_constellation_t constellation)
      : gr::block("dvb_bch_ldpc_bb",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(unsigned char))),
      Xs(0),
      P(0),
      Xp(0)
    {
      if (framesize == FECFRAME_NORMAL) {
        switch (rate) {
          case C1_4:
            kbch = 16008;
            nbch = 16200;
            bch_code = BCH_CODE_N12;
            break;
          case C1_3:
            kbch = 21408;
            nbch = 21600;
            bch_code = BCH_CODE_N12;
            break;
          case C2_5:
            kbch = 25728;
            nbch = 25920;
            bch_code = BCH_CODE_N12;
            break;
          case C1_2:
            kbch = 32208;
            nbch = 32400;
            bch_code = BCH_CODE_N12;
            break;
          case C3_5:
            kbch = 38688;
            nbch = 38880;
            bch_code = BCH_CODE_N12;
            break;
          case C2_3:
            kbch = 43040;
            nbch = 43200;
            bch_code = BCH_CODE_N10;
            break;
          case C3_4:
            kbch = 48408;
            nbch = 48600;
            bch_code = BCH_CODE_N12;
            break;
          case C4_5:
            kbch = 51648;
            nbch = 51840;
            bch_code = BCH_CODE_N12;
            break;
          case C5_6:
            kbch = 53840;
            nbch = 54000;
            bch_code = BCH_CODE_N10;
            break;
          case C8_9:
            kbch = 57472;
            nbch = 57600;
            bch_code = BCH_CODE_N8;
            break;
          case C9_10:
            kbch = 58192;
            nbch = 58320;
            bch_code = BCH_CODE_N8;
            break;
          case C2_9_VLSNR:
            kbch = 14208;
            nbch = 14400;
            bch_code = BCH_CODE_N12;
            break;
          case C13_45:
            kbch = 18528;
            nbch = 18720;
            bch_code = BCH_CODE_N12;
            break;
          case C9_20:
            kbch = 28968;
            nbch = 29160;
            bch_code = BCH_CODE_N12;
            break;
          case C90_180:
            kbch = 32208;
            nbch = 32400;
            bch_code = BCH_CODE_N12;
            break;
          case C96_180:
            kbch = 34368;
            nbch = 34560;
            bch_code = BCH_CODE_N12;
            break;
          case C11_20:
            kbch = 35448;
            nbch = 35640;
            bch_code = BCH_CODE_N12;
            break;
          case C100_180:
            kbch = 35808;
            nbch = 36000;
            bch_code = BCH_CODE_N12;
            break;
          case C104_180:
            kbch = 37248;
            nbch = 37440;
            bch_code = BCH_CODE_N12;
            break;
          case C26_45:
            kbch = 37248;
            nbch = 37440;
            bch_code = BCH_CODE_N12;
            break;
          case C18_30:
            kbch = 38688;
            nbch = 38880;
            bch_code = BCH_CODE_N12;
            break;
          case C28_45:
            kbch = 40128;
            nbch = 40320;
            bch_code = BCH_CODE_N12;
            break;
          case C23_36:
            kbch = 41208;
            nbch = 41400;
            bch_code = BCH_CODE_N12;
            break;
          case C116_180:
            kbch = 41568;
            nbch = 41760;
            bch_code = BCH_CODE_N12;
            break;
          case C20_30:
            kbch = 43008;
            nbch = 43200;
            bch_code = BCH_CODE_N12;
            break;
          case C124_180:
            kbch = 44448;
            nbch = 44640;
            bch_code = BCH_CODE_N12;
            break;
          case C25_36:
            kbch = 44808;
            nbch = 45000;
            bch_code = BCH_CODE_N12;
            break;
          case C128_180:
            kbch = 45888;
            nbch = 46080;
            bch_code = BCH_CODE_N12;
            break;
          case C13_18:
            kbch = 46608;
            nbch = 46800;
            bch_code = BCH_CODE_N12;
            break;
          case C132_180:
            kbch = 47328;
            nbch = 47520;
            bch_code = BCH_CODE_N12;
            break;
          case C22_30:
            kbch = 47328;
            nbch = 47520;
            bch_code = BCH_CODE_N12;
            break;
          case C135_180:
            kbch = 48408;
            nbch = 48600;
            bch_code = BCH_CODE_N12;
            break;
          case C140_180:
            kbch = 50208;
            nbch = 50400;
            bch_code = BCH_CODE_N12;
            break;
          case C7_9:
            kbch = 50208;
            nbch = 50400;
            bch_code = BCH_CODE_N12;
            break;
          case C154_180:
            kbch = 55248;
            nbch = 55440;
            bch_code = BCH_CODE_N12;
            break;
          default:
            kbch = 0;
            nbch = 0;
            bch_code = 0;
            break;
        }
      }
      else if (framesize == FECFRAME_SHORT) {
        switch (rate) {
          case C1_4:
            kbch = 3072;
            nbch = 3240;
            bch_code = BCH_CODE_S12;
            break;
          case C1_3:
            kbch = 5232;
            nbch = 5400;
            bch_code = BCH_CODE_S12;
            break;
          case C2_5:
            kbch = 6312;
            nbch = 6480;
            bch_code = BCH_CODE_S12;
            break;
          case C1_2:
            kbch = 7032;
            nbch = 7200;
            bch_code = BCH_CODE_S12;
            break;
          case C3_5:
            kbch = 9552;
            nbch = 9720;
            bch_code = BCH_CODE_S12;
            break;
          case C2_3:
            kbch = 10632;
            nbch = 10800;
            bch_code = BCH_CODE_S12;
            break;
          case C3_4:
            kbch = 11712;
            nbch = 11880;
            bch_code = BCH_CODE_S12;
            break;
          case C4_5:
            kbch = 12432;
            nbch = 12600;
            bch_code = BCH_CODE_S12;
            break;
          case C5_6:
            kbch = 13152;
            nbch = 13320;
            bch_code = BCH_CODE_S12;
            break;
          case C8_9:
            kbch = 14232;
            nbch = 14400;
            bch_code = BCH_CODE_S12;
            break;
          case C11_45:
            kbch = 3792;
            nbch = 3960;
            bch_code = BCH_CODE_S12;
            break;
          case C4_15:
            kbch = 4152;
            nbch = 4320;
            bch_code = BCH_CODE_S12;
            break;
          case C14_45:
            kbch = 4872;
            nbch = 5040;
            bch_code = BCH_CODE_S12;
            break;
          case C7_15:
            kbch = 7392;
            nbch = 7560;
            bch_code = BCH_CODE_S12;
            break;
          case C8_15:
            kbch = 8472;
            nbch = 8640;
            bch_code = BCH_CODE_S12;
            break;
          case C26_45:
            kbch = 9192;
            nbch = 9360;
            bch_code = BCH_CODE_S12;
            break;
          case C32_45:
            kbch = 11352;
            nbch = 11520;
            bch_code = BCH_CODE_S12;
            break;
          case C1_5_VLSNR_SF2:
            kbch = 2512;
            nbch = 2680;
            bch_code = BCH_CODE_S12;
            break;
          case C11_45_VLSNR_SF2:
            kbch = 3792;
            nbch = 3960;
            bch_code = BCH_CODE_S12;
            break;
          case C1_5_VLSNR:
            kbch = 3072;
            nbch = 3240;
            bch_code = BCH_CODE_S12;
            break;
          case C4_15_VLSNR:
            kbch = 4152;
            nbch = 4320;
            bch_code = BCH_CODE_S12;
            break;
          case C1_3_VLSNR:
            kbch = 5232;
            nbch = 5400;
            bch_code = BCH_CODE_S12;
            break;
          default:
            kbch = 0;
            nbch = 0;
            bch_code = 0;
            break;
        }
      }
      else {
        switch (rate) {
          case C1_5_MEDIUM:
            kbch = 5660;
            nbch = 5840;
            bch_code = BCH_CODE_M12;
            break;
          case C11_45_MEDIUM:
            kbch = 7740;
            nbch = 7920;
            bch_code = BCH_CODE_M12;
            break;
          case C1_3_MEDIUM:
            kbch = 10620;
            nbch = 10800;
            bch_code = BCH_CODE_M12;
            break;
          default:
            kbch = 0;
            nbch = 0;
            bch_code = 0;
            break;
        }
      }

      bch_poly_build_tables();
      //set_output_multiple(nbch);



      frame_size_type = framesize;
      if (framesize == FECFRAME_NORMAL) {
        frame_size = FRAME_SIZE_NORMAL;
        frame_size_real = FRAME_SIZE_NORMAL;
        switch (rate) {
          case C1_4:
            nbch = 16200;
            q_val = 135;
            break;
          case C1_3:
            nbch = 21600;
            q_val = 120;
            break;
          case C2_5:
            nbch = 25920;
            q_val = 108;
            break;
          case C1_2:
            nbch = 32400;
            q_val = 90;
            break;
          case C3_5:
            nbch = 38880;
            q_val = 72;
            break;
          case C2_3:
            nbch = 43200;
            q_val = 60;
            break;
          case C3_4:
            nbch = 48600;
            q_val = 45;
            break;
          case C4_5:
            nbch = 51840;
            q_val = 36;
            break;
          case C5_6:
            nbch = 54000;
            q_val = 30;
            break;
          case C8_9:
            nbch = 57600;
            q_val = 20;
            break;
          case C9_10:
            nbch = 58320;
            q_val = 18;
            break;
          case C2_9_VLSNR:
            nbch = 14400;
            q_val = 140;
            frame_size -= NORMAL_PUNCTURING;
            frame_size_real -= NORMAL_PUNCTURING;
            P = 15;
            Xp = NORMAL_PUNCTURING;
            break;
          case C13_45:
            nbch = 18720;
            q_val = 128;
            break;
          case C9_20:
            nbch = 29160;
            q_val = 99;
            break;
          case C90_180:
            nbch = 32400;
            q_val = 90;
            break;
          case C96_180:
            nbch = 34560;
            q_val = 84;
            break;
          case C11_20:
            nbch = 35640;
            q_val = 81;
            break;
          case C100_180:
            nbch = 36000;
            q_val = 80;
            break;
          case C104_180:
            nbch = 37440;
            q_val = 76;
            break;
          case C26_45:
            nbch = 37440;
            q_val = 76;
            break;
          case C18_30:
            nbch = 38880;
            q_val = 72;
            break;
          case C28_45:
            nbch = 40320;
            q_val = 68;
            break;
          case C23_36:
            nbch = 41400;
            q_val = 65;
            break;
          case C116_180:
            nbch = 41760;
            q_val = 64;
            break;
          case C20_30:
            nbch = 43200;
            q_val = 60;
            break;
          case C124_180:
            nbch = 44640;
            q_val = 56;
            break;
          case C25_36:
            nbch = 45000;
            q_val = 55;
            break;
          case C128_180:
            nbch = 46080;
            q_val = 52;
            break;
          case C13_18:
            nbch = 46800;
            q_val = 50;
            break;
          case C132_180:
            nbch = 47520;
            q_val = 48;
            break;
          case C22_30:
            nbch = 47520;
            q_val = 48;
            break;
          case C135_180:
            nbch = 48600;
            q_val = 45;
            break;
          case C140_180:
            nbch = 50400;
            q_val = 40;
            break;
          case C7_9:
            nbch = 50400;
            q_val = 40;
            break;
          case C154_180:
            nbch = 55440;
            q_val = 26;
            break;
          default:
            nbch = 0;
            q_val = 0;
            break;
        }
      }
      else if (framesize == FECFRAME_SHORT) {
        frame_size = FRAME_SIZE_SHORT;
        frame_size_real = FRAME_SIZE_SHORT;
        switch (rate) {
          case C1_4:
            nbch = 3240;
            q_val = 36;
            break;
          case C1_3:
            nbch = 5400;
            q_val = 30;
            break;
          case C2_5:
            nbch = 6480;
            q_val = 27;
            break;
          case C1_2:
            nbch = 7200;
            q_val = 25;
            break;
          case C3_5:
            nbch = 9720;
            q_val = 18;
            break;
          case C2_3:
            nbch = 10800;
            q_val = 15;
            break;
          case C3_4:
            nbch = 11880;
            q_val = 12;
            break;
          case C4_5:
            nbch = 12600;
            q_val = 10;
            break;
          case C5_6:
            nbch = 13320;
            q_val = 8;
            break;
          case C8_9:
            nbch = 14400;
            q_val = 5;
            break;
          case C11_45:
            nbch = 3960;
            q_val = 34;
            break;
          case C4_15:
            nbch = 4320;
            q_val = 33;
            break;
          case C14_45:
            nbch = 5040;
            q_val = 31;
            break;
          case C7_15:
            nbch = 7560;
            q_val = 24;
            break;
          case C8_15:
            nbch = 8640;
            q_val = 21;
            break;
          case C26_45:
            nbch = 9360;
            q_val = 19;
            break;
          case C32_45:
            nbch = 11520;
            q_val = 13;
            break;
          case C1_5_VLSNR_SF2:
            nbch = 2680;
            q_val = 135;
            frame_size -= SHORT_PUNCTURING_SET1;
            frame_size_real -= SHORT_PUNCTURING_SET1;
            Xs = 560;
            P = 30;
            Xp = 250;
            break;
          case C11_45_VLSNR_SF2:
            nbch = 3960;
            q_val = 34;
            frame_size -= SHORT_PUNCTURING_SET1;
            frame_size_real -= SHORT_PUNCTURING_SET1;
            P = 15;
            Xp = SHORT_PUNCTURING_SET1;
            break;
          case C1_5_VLSNR:
            nbch = 3240;
            q_val = 135;
            frame_size -= SHORT_PUNCTURING_SET2;
            frame_size_real -= SHORT_PUNCTURING_SET2;
            P = 10;
            Xp = SHORT_PUNCTURING_SET2;
            break;
          case C4_15_VLSNR:
            nbch = 4320;
            q_val = 33;
            frame_size -= SHORT_PUNCTURING_SET2;
            frame_size_real -= SHORT_PUNCTURING_SET2;
            P = 8;
            Xp = SHORT_PUNCTURING_SET2;
            break;
          case C1_3_VLSNR:
            nbch = 5400;
            q_val = 120;
            frame_size -= SHORT_PUNCTURING_SET2;
            frame_size_real -= SHORT_PUNCTURING_SET2;
            P = 8;
            Xp = SHORT_PUNCTURING_SET2;
            break;
          default:
            nbch = 0;
            q_val = 0;
            break;
        }
      }
      else {
        frame_size = FRAME_SIZE_MEDIUM - MEDIUM_PUNCTURING;
        frame_size_real = FRAME_SIZE_MEDIUM - MEDIUM_PUNCTURING;
        switch (rate) {
          case C1_5_MEDIUM:
            nbch = 5840;
            q_val = 72;
            Xs = 640;
            P = 25;
            Xp = 980;
            break;
          case C11_45_MEDIUM:
            nbch = 7920;
            q_val = 68;
            P = 15;
            Xp = MEDIUM_PUNCTURING;
            break;
          case C1_3_MEDIUM:
            nbch = 10800;
            q_val = 60;
            P = 13;
            Xp = MEDIUM_PUNCTURING;
            break;
          default:
            nbch = 0;
            q_val = 0;
            break;
        }
      }
      code_rate = rate;
      signal_constellation = constellation;
      dvb_standard = standard;
      ldpc_lookup_generate();
      if (signal_constellation == MOD_128APSK) {
        frame_size += 6;
      }
      set_output_multiple(frame_size);
    }

    /*
     * Our virtual destructor.
     */
    dvb_bch_ldpc_bb_impl::~dvb_bch_ldpc_bb_impl()
    {
    }

    void
    dvb_bch_ldpc_bb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = (noutput_items / frame_size * kbch); //kbch input items per frame_size output items
      // ninput_items_required[0] = (noutput_items / nbch) * kbch;
      // ninput_items_required[0] = (noutput_items / frame_size) * nbch;
    }

    //Every 'im' needs to be bch'd only once!
    #define LDPC_BF(TABLE_NAME, ROWS) \
    for (int row = 0; row < ROWS; row++) { \
      for (int n = 0; n < 360; n++) { \
        bchd = false; \
        for (int col = 1; col <= TABLE_NAME[row][0]; col++) { \
          ldpc_encode.item[index].p = (TABLE_NAME[row][col] + (n * q)) % pbits; \
          ldpc_encode.item[index].d = im; \
          if(!bchd && (ldpc_encode.item[index].p < (int)kbch && ldpc_encode.item[index].d < (int)kbch)) { \
            bchd = true; \
            ldpc_encode.item_bch[index_bch].p = (TABLE_NAME[row][col] + (n * q)) % pbits; \
            ldpc_encode.item_bch[index_bch].d = im; \
            index_bch++; \
          } else { \
            ldpc_encode.item_nobch[index_nobch].p = (TABLE_NAME[row][col] + (n * q)) % pbits; \
            ldpc_encode.item_nobch[index_nobch].d = im; \
            index_nobch++; \
          } \
          index++; \
        } \
        im++; \
      } \
    }


    void
    dvb_bch_ldpc_bb_impl::ldpc_lookup_generate(void)
    {
      int im;
      int index, index_bch, index_nobch;
      int pbits;
      int q;
      bool bchd;
      index = 0;
      index_bch = 0;
      index_nobch = 0;
      im = 0;

      pbits = (frame_size_real + Xp) - nbch;    //number of parity bits
      q = q_val;

      if (frame_size_type == FECFRAME_NORMAL) {
        if (code_rate == C1_4) {
          LDPC_BF(ldpc_tab_1_4N,  45);
        }
        if (code_rate == C1_3) {
          LDPC_BF(ldpc_tab_1_3N,  60);
        }
        if (code_rate == C2_5) {
          LDPC_BF(ldpc_tab_2_5N,  72);
        }
        if (code_rate == C1_2) {
          LDPC_BF(ldpc_tab_1_2N,  90);
        }
        if (code_rate == C3_5) {
          LDPC_BF(ldpc_tab_3_5N,  108);
        }
        if (code_rate == C2_3) {
          if (dvb_standard == STANDARD_DVBT2) {
            LDPC_BF(ldpc_tab_2_3N_DVBT2, 120);
          }
          else {
            LDPC_BF(ldpc_tab_2_3N_DVBS2, 120);
          }
        }
        if (code_rate == C3_4) {
          LDPC_BF(ldpc_tab_3_4N,  135);
        }
        if (code_rate == C4_5) {
          LDPC_BF(ldpc_tab_4_5N,  144);
        }
        if (code_rate == C5_6) {
          LDPC_BF(ldpc_tab_5_6N,  150);
        }
        if (code_rate == C8_9) {
          LDPC_BF(ldpc_tab_8_9N,  160);
        }
        if (code_rate == C9_10) {
          LDPC_BF(ldpc_tab_9_10N, 162);
        }
        if (code_rate == C2_9_VLSNR) {
          LDPC_BF(ldpc_tab_2_9N,      40);
        }
        if (code_rate == C13_45) {
          LDPC_BF(ldpc_tab_13_45N,    52);
        }
        if (code_rate == C9_20) {
          LDPC_BF(ldpc_tab_9_20N,     81);
        }
        if (code_rate == C90_180) {
          LDPC_BF(ldpc_tab_90_180N,   90);
        }
        if (code_rate == C96_180) {
          LDPC_BF(ldpc_tab_96_180N,   96);
        }
        if (code_rate == C11_20) {
          LDPC_BF(ldpc_tab_11_20N,    99);
        }
        if (code_rate == C100_180) {
          LDPC_BF(ldpc_tab_100_180N, 100);
        }
        if (code_rate == C104_180) {
          LDPC_BF(ldpc_tab_104_180N, 104);
        }
        if (code_rate == C26_45) {
          LDPC_BF(ldpc_tab_26_45N,   104);
        }
        if (code_rate == C18_30) {
          LDPC_BF(ldpc_tab_18_30N,   108);
        }
        if (code_rate == C28_45) {
          LDPC_BF(ldpc_tab_28_45N,   112);
        }
        if (code_rate == C23_36) {
          LDPC_BF(ldpc_tab_23_36N,   115);
        }
        if (code_rate == C116_180) {
          LDPC_BF(ldpc_tab_116_180N, 116);
        }
        if (code_rate == C20_30) {
          LDPC_BF(ldpc_tab_20_30N,   120);
        }
        if (code_rate == C124_180) {
          LDPC_BF(ldpc_tab_124_180N, 124);
        }
        if (code_rate == C25_36) {
          LDPC_BF(ldpc_tab_25_36N,   125);
        }
        if (code_rate == C128_180) {
          LDPC_BF(ldpc_tab_128_180N, 128);
        }
        if (code_rate == C13_18) {
          LDPC_BF(ldpc_tab_13_18N,   130);
        }
        if (code_rate == C132_180) {
          LDPC_BF(ldpc_tab_132_180N, 132);
        }
        if (code_rate == C22_30) {
          LDPC_BF(ldpc_tab_22_30N,   132);
        }
        if (code_rate == C135_180) {
          LDPC_BF(ldpc_tab_135_180N, 135);
        }
        if (code_rate == C140_180) {
          LDPC_BF(ldpc_tab_140_180N, 140);
        }
        if (code_rate == C7_9) {
          LDPC_BF(ldpc_tab_7_9N,     140);
        }
        if (code_rate == C154_180) {
          LDPC_BF(ldpc_tab_154_180N, 154);
        }
      }
      else if (frame_size_type == FECFRAME_SHORT) {
        if (code_rate == C1_4) {
          LDPC_BF(ldpc_tab_1_4S, 9);
        }
        if (code_rate == C1_3) {
          LDPC_BF(ldpc_tab_1_3S, 15);
        }
        if (code_rate == C2_5) {
          LDPC_BF(ldpc_tab_2_5S, 18);
        }
        if (code_rate == C1_2) {
          LDPC_BF(ldpc_tab_1_2S, 20);
        }
        if (code_rate == C3_5) {
          if (dvb_standard == STANDARD_DVBT2) {
            LDPC_BF(ldpc_tab_3_5S_DVBT2, 27);
          }
          else {
            LDPC_BF(ldpc_tab_3_5S_DVBS2, 27);
          }
        }
        if (code_rate == C2_3) {
          LDPC_BF(ldpc_tab_2_3S, 30);
        }
        if (code_rate == C3_4) {
          LDPC_BF(ldpc_tab_3_4S, 33);
        }
        if (code_rate == C4_5) {
          LDPC_BF(ldpc_tab_4_5S, 35);
        }
        if (code_rate == C5_6) {
          LDPC_BF(ldpc_tab_5_6S, 37);
        }
        if (code_rate == C8_9) {
          LDPC_BF(ldpc_tab_8_9S, 40);
        }
        if (code_rate == C11_45) {
          LDPC_BF(ldpc_tab_11_45S, 11);
        }
        if (code_rate == C4_15) {
          LDPC_BF(ldpc_tab_4_15S,  12);
        }
        if (code_rate == C14_45) {
          LDPC_BF(ldpc_tab_14_45S, 14);
        }
        if (code_rate == C7_15) {
          LDPC_BF(ldpc_tab_7_15S,  21);
        }
        if (code_rate == C8_15) {
          LDPC_BF(ldpc_tab_8_15S,  24);
        }
        if (code_rate == C26_45) {
          LDPC_BF(ldpc_tab_26_45S, 26);
        }
        if (code_rate == C32_45) {
          LDPC_BF(ldpc_tab_32_45S, 32);
        }
        if (code_rate == C1_5_VLSNR_SF2) {
          LDPC_BF(ldpc_tab_1_4S,    9);
        }
        if (code_rate == C11_45_VLSNR_SF2) {
          LDPC_BF(ldpc_tab_11_45S, 11);
        }
        if (code_rate == C1_5_VLSNR) {
          LDPC_BF(ldpc_tab_1_4S,    9);
        }
        if (code_rate == C4_15_VLSNR) {
          LDPC_BF(ldpc_tab_4_15S,  12);
        }
        if (code_rate == C1_3_VLSNR) {
          LDPC_BF(ldpc_tab_1_3S,   15);
        }
      }
      else {
        if (code_rate == C1_5_MEDIUM) {
          LDPC_BF(ldpc_tab_1_5M,   18);
        }
        if (code_rate == C11_45_MEDIUM) {
          LDPC_BF(ldpc_tab_11_45M, 22);
        }
        if (code_rate == C1_3_MEDIUM) {
          LDPC_BF(ldpc_tab_1_3M,   30);
        }
      }
      ldpc_encode.table_length = index;
      ldpc_encode.table_length_nobch = index_nobch;
      ldpc_encode.table_length_bch = index_bch;
    }

    int
    dvb_bch_ldpc_bb_impl::general_work_ldpc (int noutput_items,
                       gr_vector_int &ninput_items,
                       unsigned char *in_ldpc,
                       gr_vector_void_star &output_items)
    {
      unsigned char *out_ldpc = (unsigned char *) output_items[0];
      const unsigned char *d;
      unsigned char *p;
      unsigned char *b = (unsigned char *) output_items[0];
      unsigned char *s;
      // Calculate the number of parity bits
      int plen = (frame_size_real + Xp) - nbch;
      d = in_ldpc;
      p = &out_ldpc[nbch];
      int consumed = 0;
      int puncture, index;

      for (int i = 0; i < noutput_items; i += frame_size) {
        if (Xs != 0) {
          s = &shortening_buffer[0];
          memset(s, 0, sizeof(unsigned char) * Xs);
          memcpy(&s[Xs], &in_ldpc[consumed], sizeof(unsigned char) * nbch);
          d = s;
        }
        if (P != 0) {
          p = &puncturing_buffer[nbch];
          b = &out_ldpc[i + nbch];
        }
        // First zero all the parity bits
        memset(p, 0, sizeof(unsigned char) * plen);

        //use memcpy instead of the for loop
        memcpy(&out_ldpc[i],&in_ldpc[consumed],sizeof(unsigned char) * (int)nbch);
        consumed += (int)nbch;

        //nbch = 38880;
        // frame_size = 64800;

        // now do the parity checking
        for (int j = 0; j < ldpc_encode.table_length; j++) {
          p[ldpc_encode.item[j].p] ^= d[ldpc_encode.item[j].d];
        }
        if (P != 0) {
          puncture = 0;
          for (int j = 0; j < plen; j += P) {
            p[j] = 0x55;
            puncture++;
            if (puncture == Xp) {
              break;
            }
          }
          index = 0;
          for (int j = 0; j < plen; j++) {
            if (p[j] != 0x55) {
              b[index++] = p[j];
            }
          }
          p = &out_ldpc[nbch];
        }


        for (int j = 1; j < (plen - Xp); j++) {
          p[j] ^= p[j - 1];
        }

        if (signal_constellation == MOD_128APSK) {
          for (int j = 0; j < 6; j++) {
            p[j + plen] = 0;
          }
        }
        d += nbch;
        p += frame_size;
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }


    /*
     * Polynomial calculation routines
     * multiply polynomials
     */
    int
    dvb_bch_ldpc_bb_impl::poly_mult(const int *ina, int lena, const int *inb, int lenb, int *out)
    {


      memset(out, 0, sizeof(int) * (lena + lenb));

      for (int i = 0; i < lena; i++) {
        for (int j = 0; j < lenb; j++) {
          if (ina[i] * inb[j] > 0 ) {
            out[i + j]++;    // count number of terms for this pwr of x
          }
        }
      }
      int max = 0;
      for (int i = 0; i < lena + lenb; i++) {
        out[i] = out[i] & 1;    // If even ignore the term
        if(out[i]) {
          max = i;
        }
      }
      // return the size of array to house the result.
      return max + 1;
    }

    /*
     * Pack the polynomial into a 32 bit array
     */
    void
    dvb_bch_ldpc_bb_impl::poly_pack(const int *pin, unsigned int* pout, int len)
    {
      int lw = len / 32;
      int ptr = 0;
      unsigned int temp;
      if (len % 32) {
        lw++;
      }

      for (int i = 0; i < lw; i++) {
        temp = 0x80000000;
        pout[i] = 0;
        for (int j = 0; j < 32; j++) {
          if (pin[ptr++]) {
            pout[i] |= temp;
          }
          temp >>= 1;
        }
      }
    }

    void
    dvb_bch_ldpc_bb_impl::poly_reverse(int *pin, int *pout, int len)
    {
      int c;
      c = len - 1;

      for (int i = 0; i < len; i++) {
        pout[c--] = pin[i];
      }
    }

    /*
     *Shift a 128 bit register
     */
    inline void
    dvb_bch_ldpc_bb_impl::reg_4_shift(unsigned int *sr)
    {
      sr[3] = (sr[3] >> 1) | (sr[2] << 31);
      sr[2] = (sr[2] >> 1) | (sr[1] << 31);
      sr[1] = (sr[1] >> 1) | (sr[0] << 31);
      sr[0] = (sr[0] >> 1);
    }

    /*
     * Shift 160 bits
     */
    inline void
    dvb_bch_ldpc_bb_impl::reg_5_shift(unsigned int *sr)
    {
      sr[4] = (sr[4] >> 1) | (sr[3] << 31);
      sr[3] = (sr[3] >> 1) | (sr[2] << 31);
      sr[2] = (sr[2] >> 1) | (sr[1] << 31);
      sr[1] = (sr[1] >> 1) | (sr[0] << 31);
      sr[0] = (sr[0] >> 1);
    }

    /*
     * Shift 192 bits
     */
    inline void
    dvb_bch_ldpc_bb_impl::reg_6_shift(unsigned int *sr)
    {
      sr[5] = (sr[5] >> 1) | (sr[4] << 31);
      sr[4] = (sr[4] >> 1) | (sr[3] << 31);
      sr[3] = (sr[3] >> 1) | (sr[2] << 31);
      sr[2] = (sr[2] >> 1) | (sr[1] << 31);
      sr[1] = (sr[1] >> 1) | (sr[0] << 31);
      sr[0] = (sr[0] >> 1);
    }

    #if defined(__AVX2__)
        //Modified from http://stackoverflow.com/questions/25248766/emulating-shifts-on-32-bytes-with-avx
        //----------------------------------------------------------------------------
        // bit shift right a 256-bit value using ymm registers
        //          __m256i *data - data to shift
        //          int count     - number of bits to shift
        // return:  __m256i       - carry out bit(s)
        inline bool dvb_bch_ldpc_bb_impl::bitShiftRight256ymm (__m256i *data, int count)
           {
           __m256i innerCarry, carryOut, rotate;

           //innerCarry = _mm256_set_epi32(0,1,2,4,8,16,64,128);

           innerCarry = _mm256_slli_epi64 (*data, 64 - count);                        // carry outs in bit (64-count) of each qword
           rotate     = _mm256_permute4x64_epi64 (innerCarry, 0b00111001);            // rotate ymm RIGHT 64 bits (left was 0x93=0b10 01 00 11). Crosslane operation, may be slow
           //innerCarry = _mm256_blend_epi32 (_mm256_setzero_si256 (), rotate, 0xFC); // clear highest qword
           //blend chooses from either first or second operand, depending on third.
           //0xFC is 0b11111100 (left) --> modify to 0b00111111 (right)
           innerCarry = _mm256_blend_epi32 (_mm256_setzero_si256 (), rotate, 0b00111111);
           *data      = _mm256_srli_epi64 (*data, count);                             // shift all qwords left
           *data      = _mm256_or_si256 (*data, innerCarry);                          // propagate carrys
           carryOut   = _mm256_xor_si256 (innerCarry, rotate);                        // clear all except highest qword
           return !_mm256_testz_si256(carryOut,carryOut); //p1 & p2 == 0
           }

        //----------------------------------------------------------------------------

        //http://stackoverflow.com/questions/746171/best-algorithm-for-bit-reversal-from-msb-lsb-to-lsb-msb-in-c
        //not needed
        inline unsigned int
        dvb_bch_ldpc_bb_impl::reverse(register unsigned int x)
        {
            x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
            x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
            x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
            x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
            return((x >> 16) | (x << 16));
        }
    #endif

    int
    dvb_bch_ldpc_bb_impl::general_work_bch (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       unsigned char *out_bch)
    {
      const unsigned char *in_bch = (const unsigned char *) input_items[0];
      unsigned char b, temp;
      unsigned int shift[6];
      int consumed = 0;

      #if defined(__AVX2__)
      //Should do this only once, but causes segfault..
      __m256i m_256_poly_n_12 = _mm256_set_epi32(0,
                                                0,
                                                m_poly_n_12[0],
                                                m_poly_n_12[1],
                                                m_poly_n_12[2],
                                                m_poly_n_12[3],
                                                m_poly_n_12[4],
                                                m_poly_n_12[5]);
      #endif

      switch (bch_code) {
        case BCH_CODE_N12:
          //TODO: Make this pretty, use VOLK
          #if defined(__AVX2__)
          //#if false
            #warning "USING AVX2"
            {
            for (int i = 0; i < noutput_items; i += nbch) {
              //Zero the shift register
              __m256i shift_vector = _mm256_setzero_si256();
              // MSB of the codeword first
              for (int j = 0; j < (int)kbch; j++) {
                temp = *in_bch++;
                *out_bch++ = temp;
                bool carry = bitShiftRight256ymm(&shift_vector,1);
                b = temp ^ carry; //(((int*) &carry)[7] != 0);
                if (b) {
                  shift_vector = _mm256_xor_si256(shift_vector, m_256_poly_n_12);
                }
              }
              // Now add the parity bits to the output

              //for (int n = 0; n < 192; n++) {
              //  __m256i carry = bitShiftRight256ymm(&shift_vector,1);
              //  *out_bch++ = (((int*) &carry)[7] != 0);
              //}
              //More efficient:
              for(int n = 0; n < 6; n++) {
                unsigned int shift_int = (((unsigned int*)&shift_vector)[n]);
                for(int m = 0; m < 32; m++) {
                  *out_bch++ = (char)(shift_int & 1);
                  shift_int >>= 1;
                }
              }
            }
            consumed += (int)kbch;
          }
          #else
            #warning "NOT USING AVX2"
            for (int i = 0; i < noutput_items; i += nbch) {
              //Zero the shift register
              memset(shift, 0, sizeof(unsigned int) * 6);
              // MSB of the codeword first
              for (int j = 0; j < (int)kbch; j++) {
                temp = *in_bch++;
                *out_bch++ = temp;
                consumed++;
                b = (temp ^ (shift[5] & 1));
                reg_6_shift(shift);
                if (b) {
                  shift[0] ^= m_poly_n_12[0];
                  shift[1] ^= m_poly_n_12[1];
                  shift[2] ^= m_poly_n_12[2];
                  shift[3] ^= m_poly_n_12[3];
                  shift[4] ^= m_poly_n_12[4];
                  shift[5] ^= m_poly_n_12[5];
                }
              }
              // Now add the parity bits to the output
              /*for (int n = 0; n < 192; n++) {
                *out_bch++ = (shift[5] & 1);
                reg_6_shift(shift);
              }*/
              for(int n = 0; n < 6; n++) {
                unsigned int shift_int = shift[5-n];
                for(int m = 0; m < 32; m++) {
                  *out_bch++ = (char)(shift_int & 1);
                  shift_int >>= 1;
                }
              }
            }
          #endif

          break;
        case BCH_CODE_N10:
          for (int i = 0; i < noutput_items; i += nbch) {
            //Zero the shift register
            memset(shift, 0, sizeof(unsigned int) * 5);
            // MSB of the codeword first
            for (int j = 0; j < (int)kbch; j++) {
              temp = *in_bch++;
              *out_bch++ = temp;
              consumed++;
              b = (temp ^ (shift[4] & 1));
              reg_5_shift(shift);
              if (b) {
                shift[0] ^= m_poly_n_10[0];
                shift[1] ^= m_poly_n_10[1];
                shift[2] ^= m_poly_n_10[2];
                shift[3] ^= m_poly_n_10[3];
                shift[4] ^= m_poly_n_10[4];
              }
            }
            // Now add the parity bits to the output
            for( int n = 0; n < 160; n++ ) {
              *out_bch++ = (shift[4] & 1);
              reg_5_shift(shift);
            }
          }
          break;
        case BCH_CODE_N8:
          for (int i = 0; i < noutput_items; i += nbch) {
            //Zero the shift register
            memset(shift, 0, sizeof(unsigned int) * 4);
            // MSB of the codeword first
            for (int j = 0; j < (int)kbch; j++) {
              temp = *in_bch++;
              *out_bch++ = temp;
              consumed++;
              b = temp ^ (shift[3] & 1);
              reg_4_shift(shift);
              if (b) {
                shift[0] ^= m_poly_n_8[0];
                shift[1] ^= m_poly_n_8[1];
                shift[2] ^= m_poly_n_8[2];
                shift[3] ^= m_poly_n_8[3];
              }
            }
            // Now add the parity bits to the output
            for (int n = 0; n < 128; n++) {
              *out_bch++ = shift[3] & 1;
              reg_4_shift(shift);
            }
          }
          break;
        case BCH_CODE_S12:
          for (int i = 0; i < noutput_items; i += nbch) {
            //Zero the shift register
            memset(shift, 0, sizeof(unsigned int) * 6);
            // MSB of the codeword first
            for (int j = 0; j < (int)kbch; j++) {
              temp = *in_bch++;
              *out_bch++ = temp;
              consumed++;
              b = (temp ^ ((shift[5] & 0x01000000) ? 1 : 0));
              reg_6_shift(shift);
              if (b) {
                shift[0] ^= m_poly_s_12[0];
                shift[1] ^= m_poly_s_12[1];
                shift[2] ^= m_poly_s_12[2];
                shift[3] ^= m_poly_s_12[3];
                shift[4] ^= m_poly_s_12[4];
                shift[5] ^= m_poly_s_12[5];
              }
            }
            // Now add the parity bits to the output
            for (int n = 0; n < 168; n++) {
              *out_bch++ = (shift[5] & 0x01000000) ? 1 : 0;
              reg_6_shift(shift);
            }
          }
          break;
        case BCH_CODE_M12:
          for (int i = 0; i < noutput_items; i += nbch) {
            //Zero the shift register
            memset(shift, 0, sizeof(unsigned int) * 6);
            // MSB of the codeword first
            for (int j = 0; j < (int)kbch; j++) {
              temp = *in_bch++;
              *out_bch++ = temp;
              consumed++;
              b = (temp ^ ((shift[5] & 0x00001000) ? 1 : 0));
              reg_6_shift(shift);
              if (b) {
                shift[0] ^= m_poly_m_12[0];
                shift[1] ^= m_poly_m_12[1];
                shift[2] ^= m_poly_m_12[2];
                shift[3] ^= m_poly_m_12[3];
                shift[4] ^= m_poly_m_12[4];
                shift[5] ^= m_poly_m_12[5];
              }
            }
            // Now add the parity bits to the output
            for (int n = 0; n < 180; n++) {
              *out_bch++ = (shift[5] & 0x00001000) ? 1 : 0;
              reg_6_shift(shift);
            }
          }
          break;
      }

      // Tell runtime system how many input items we consumed on
      // each input stream.
      // consume_each ((int) kbch);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }


    int
    dvb_bch_ldpc_bb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        //(noutput_items / nbch) * kbch
        //noutput_items == multiple of frame_size
        //frame_size = Nldpc = kldpc + parity bits
        //ldpc input = kldpc = nbch
        //nbch = kbch + parity bits
        //gr_vector_void_star temp_items = (gr_vector_void_star)malloc(sizeof(char)*);
        //gr_vector_void_star temp_items(noutput_items);
        //temp_items.reserve(noutput_items/frame_size*nbch);



        //general_work_bch(noutput_items/frame_size*kbch, ninput_items, input_items, temp_items);

        const unsigned char *in_bch = (const unsigned char *) input_items[0];
        unsigned char b_bch, temp;
        unsigned int shift[6];
        int consumed = 0;
        int noutput_items_bch = noutput_items/frame_size*nbch; //TODO: ==??

        unsigned char *out_ldpc = (unsigned char *) output_items[0];
        const unsigned char *d;
        unsigned char *p;
        unsigned char *b = (unsigned char *) output_items[0];
        unsigned char *s;
        // Calculate the number of ldpc parity bits
        int plen = (frame_size_real + Xp) - nbch;

        #if defined(__AVX2__)
        //Should do this only once, but causes segfault..
        __m256i m_256_poly_n_12 = _mm256_set_epi32(0,
                                                  0,
                                                  m_poly_n_12[0],
                                                  m_poly_n_12[1],
                                                  m_poly_n_12[2],
                                                  m_poly_n_12[3],
                                                  m_poly_n_12[4],
                                                  m_poly_n_12[5]);
        #endif




        //Fallback for 'other' cases (not the competition case)
        if(bch_code == BCH_CODE_N12) {
          //TODO: Make this pretty, use VOLK
          #if defined(__AVX2__)
          //#if false
            #warning "USING AVX2"
            {
              d = out_ldpc;
              p = &out_ldpc[nbch];
              int puncture, index;

              for (int i = 0; i < noutput_items; i += frame_size) {
                if (Xs != 0) {
                  s = &shortening_buffer[0];
                  memset(s, 0, sizeof(unsigned char) * Xs);
                  memcpy(&s[Xs], &out_ldpc[consumed], sizeof(unsigned char) * nbch);
                  d = s;
                }
                if (P != 0) {
                  p = &puncturing_buffer[nbch];
                  b = &out_ldpc[i + nbch];
                }

                //Zero all the LDPC parity bits
                memset(p, 0, sizeof(unsigned char) * plen);

                //Zero the shift register for BCH Code
                __m256i shift_vector = _mm256_setzero_si256();

                //copy input samples to output
                memcpy(&out_ldpc[i],&in_bch[consumed],sizeof(unsigned char) * (int)kbch); //Missing last nbch - kbch bits, but not available yet!

                // now do the parity checking of first kbch bits and generate ldpc parity
                bool carry;
                for (int j = 0; j < ldpc_encode.table_length_bch; j++) {
                  temp = d[ldpc_encode.item_bch[j].d];
                  p[ldpc_encode.item_bch[j].p] ^= temp;
                  carry = bitShiftRight256ymm(&shift_vector,1);
                  b_bch = temp ^ carry; //(((int*) &carry)[7] != 0);
                  if (b_bch) {
                    shift_vector = _mm256_xor_si256(shift_vector, m_256_poly_n_12);
                  }
                }

                // Now add the bch parity bits to the output (add the missing nbch - kbch bits)
                unsigned char *bch_parity = &out_ldpc[i]+kbch;
                for(int n = 0; n < 6; n++) {
                  unsigned int shift_int = (((unsigned int*)&shift_vector)[n]);
                  for(int m = 0; m < 32; m++) {
                    *bch_parity++ = (char)(shift_int & 1);
                    shift_int >>= 1;
                  }
                }
                // continue the ldpc parity checking
                for (int j = 0; j < ldpc_encode.table_length_nobch; j++) {
                  p[ldpc_encode.item_nobch[j].p] ^= d[ldpc_encode.item_nobch[j].d];
                }

                if (P != 0) {
                  puncture = 0;
                  for (int j = 0; j < plen; j += P) {
                    p[j] = 0x55;
                    puncture++;
                    if (puncture == Xp) {
                      break;
                    }
                  }
                  index = 0;
                  for (int j = 0; j < plen; j++) {
                    if (p[j] != 0x55) {
                      b[index++] = p[j];
                    }
                  }
                  p = &out_ldpc[nbch];
                }

                for (int j = 1; j < (plen - Xp); j++) {
                  p[j] ^= p[j - 1];
                }

                if (signal_constellation == MOD_128APSK) {
                  for (int j = 0; j < 6; j++) {
                    p[j + plen] = 0;
                  }
                }
                d += nbch;
                p += frame_size;
                consumed += (int)kbch;
            }
          }
          #else
            #warning "NOT USING AVX2"
            d = out_ldpc;
            p = &out_ldpc[nbch];
            int puncture, index;

            for (int i = 0; i < noutput_items; i += frame_size) {
              if (Xs != 0) {
                s = &shortening_buffer[0];
                memset(s, 0, sizeof(unsigned char) * Xs);
                memcpy(&s[Xs], &out_ldpc[consumed], sizeof(unsigned char) * nbch);
                d = s;
              }
              if (P != 0) {
                p = &puncturing_buffer[nbch];
                b = &out_ldpc[i + nbch];
              }

              //Zero all the LDPC parity bits
              memset(p, 0, sizeof(unsigned char) * plen);

              //Zero the shift register for BCH Code
              memset(shift, 0, sizeof(unsigned int) * 6);

              //copy input samples to output
              memcpy(&out_ldpc[i],&in_bch[consumed],sizeof(unsigned char) * (int)kbch); //Missing last nbch - kbch bits, but not available yet!

              // now do the parity checking of first kbch bits
              for (int j = 0; j < ldpc_encode.table_length_bch; j++) {
                temp = d[ldpc_encode.item_bch[j].d];
                p[ldpc_encode.item_bch[j].p] ^= temp;
                b_bch = (temp ^ (shift[5] & 1));
                reg_6_shift(shift);
                if (b_bch) {
                  shift[0] ^= m_poly_n_12[0];
                  shift[1] ^= m_poly_n_12[1];
                  shift[2] ^= m_poly_n_12[2];
                  shift[3] ^= m_poly_n_12[3];
                  shift[4] ^= m_poly_n_12[4];
                  shift[5] ^= m_poly_n_12[5];
                }
              }

              // Now add the bch parity bits to the output (add the missing nbch - kbch bits)
              unsigned char *bch_parity = &out_ldpc[i]+kbch;
              for(int n = 0; n < 6; n++) {
                unsigned int shift_int = shift[5-n];
                for(int m = 0; m < 32; m++) {
                  *bch_parity++ = (char)(shift_int & 1);
                  shift_int >>= 1;
                }
              }

              // continue the parity checking
              for (int j = 0; j < ldpc_encode.table_length_nobch; j++) {
                p[ldpc_encode.item_nobch[j].p] ^= d[ldpc_encode.item_nobch[j].d];
              }

              if (P != 0) {
                puncture = 0;
                for (int j = 0; j < plen; j += P) {
                  p[j] = 0x55;
                  puncture++;
                  if (puncture == Xp) {
                    break;
                  }
                }
                index = 0;
                for (int j = 0; j < plen; j++) {
                  if (p[j] != 0x55) {
                    b[index++] = p[j];
                  }
                }
                p = &out_ldpc[nbch];
              }

              for (int j = 1; j < (plen - Xp); j++) {
                p[j] ^= p[j - 1];
              }

              if (signal_constellation == MOD_128APSK) {
                for (int j = 0; j < 6; j++) {
                  p[j + plen] = 0;
                }
              }
              d += nbch;
              p += frame_size;
              consumed += (int)kbch;
            }
          #endif
        }
        else
        {
          unsigned char *out_bch = (unsigned char*) malloc(sizeof(unsigned char)*noutput_items);
          unsigned char *in_ldpc = out_bch;

          switch (bch_code) {
            case BCH_CODE_N12:
                //implemented above
              break;
            case BCH_CODE_N10:
              for (int i = 0; i < noutput_items_bch; i += nbch) {
                //Zero the shift register
                memset(shift, 0, sizeof(unsigned int) * 5);
                // MSB of the codeword first
                for (int j = 0; j < (int)kbch; j++) {
                  temp = *in_bch++;
                  *out_bch++ = temp;
                  consumed++;
                  b_bch = (temp ^ (shift[4] & 1));
                  reg_5_shift(shift);
                  if (b_bch) {
                    shift[0] ^= m_poly_n_10[0];
                    shift[1] ^= m_poly_n_10[1];
                    shift[2] ^= m_poly_n_10[2];
                    shift[3] ^= m_poly_n_10[3];
                    shift[4] ^= m_poly_n_10[4];
                  }
                }
                // Now add the parity bits to the output
                for( int n = 0; n < 160; n++ ) {
                  *out_bch++ = (shift[4] & 1);
                  reg_5_shift(shift);
                }
              }
              break;
            case BCH_CODE_N8:
              for (int i = 0; i < noutput_items_bch; i += nbch) {
                //Zero the shift register
                memset(shift, 0, sizeof(unsigned int) * 4);
                // MSB of the codeword first
                for (int j = 0; j < (int)kbch; j++) {
                  temp = *in_bch++;
                  *out_bch++ = temp;
                  consumed++;
                  b_bch = temp ^ (shift[3] & 1);
                  reg_4_shift(shift);
                  if (b_bch) {
                    shift[0] ^= m_poly_n_8[0];
                    shift[1] ^= m_poly_n_8[1];
                    shift[2] ^= m_poly_n_8[2];
                    shift[3] ^= m_poly_n_8[3];
                  }
                }
                // Now add the parity bits to the output
                for (int n = 0; n < 128; n++) {
                  *out_bch++ = shift[3] & 1;
                  reg_4_shift(shift);
                }
              }
              break;
            case BCH_CODE_S12:
              for (int i = 0; i < noutput_items_bch; i += nbch) {
                //Zero the shift register
                memset(shift, 0, sizeof(unsigned int) * 6);
                // MSB of the codeword first
                for (int j = 0; j < (int)kbch; j++) {
                  temp = *in_bch++;
                  *out_bch++ = temp;
                  consumed++;
                  b_bch = (temp ^ ((shift[5] & 0x01000000) ? 1 : 0));
                  reg_6_shift(shift);
                  if (b_bch) {
                    shift[0] ^= m_poly_s_12[0];
                    shift[1] ^= m_poly_s_12[1];
                    shift[2] ^= m_poly_s_12[2];
                    shift[3] ^= m_poly_s_12[3];
                    shift[4] ^= m_poly_s_12[4];
                    shift[5] ^= m_poly_s_12[5];
                  }
                }
                // Now add the parity bits to the output
                for (int n = 0; n < 168; n++) {
                  *out_bch++ = (shift[5] & 0x01000000) ? 1 : 0;
                  reg_6_shift(shift);
                }
              }
              break;
            case BCH_CODE_M12:
              for (int i = 0; i < noutput_items_bch; i += nbch) {
                //Zero the shift register
                memset(shift, 0, sizeof(unsigned int) * 6);
                // MSB of the codeword first
                for (int j = 0; j < (int)kbch; j++) {
                  temp = *in_bch++;
                  *out_bch++ = temp;
                  consumed++;
                  b_bch = (temp ^ ((shift[5] & 0x00001000) ? 1 : 0));
                  reg_6_shift(shift);
                  if (b_bch) {
                    shift[0] ^= m_poly_m_12[0];
                    shift[1] ^= m_poly_m_12[1];
                    shift[2] ^= m_poly_m_12[2];
                    shift[3] ^= m_poly_m_12[3];
                    shift[4] ^= m_poly_m_12[4];
                    shift[5] ^= m_poly_m_12[5];
                  }
                }
                // Now add the parity bits to the output
                for (int n = 0; n < 180; n++) {
                  *out_bch++ = (shift[5] & 0x00001000) ? 1 : 0;
                  reg_6_shift(shift);
                }
              }
              break;
          }

          d = in_ldpc;
          p = &out_ldpc[nbch];
          int puncture, index;
          consumed = 0;
          for (int i = 0; i < noutput_items; i += frame_size) {
            if (Xs != 0) {
              s = &shortening_buffer[0];
              memset(s, 0, sizeof(unsigned char) * Xs);
              memcpy(&s[Xs], &in_ldpc[consumed], sizeof(unsigned char) * nbch);
              d = s;
            }
            if (P != 0) {
              p = &puncturing_buffer[nbch];
              b = &out_ldpc[i + nbch];
            }

            // First zero all the parity bits
            memset(p, 0, sizeof(unsigned char) * plen);

            //use memcpy instead of the for loop
            memcpy(&out_ldpc[i],&in_ldpc[consumed],sizeof(unsigned char) * (int)nbch);
            consumed += (int)nbch;

            // nbch = 38880;
            // frame_size = 64800;

            // now do the parity checking
            for (int j = 0; j < ldpc_encode.table_length; j++) {
              p[ldpc_encode.item[j].p] ^= d[ldpc_encode.item[j].d];
            }
            if (P != 0) {
              puncture = 0;
              for (int j = 0; j < plen; j += P) {
                p[j] = 0x55;
                puncture++;
                if (puncture == Xp) {
                  break;
                }
              }
              index = 0;
              for (int j = 0; j < plen; j++) {
                if (p[j] != 0x55) {
                  b[index++] = p[j];
                }
              }
              p = &out_ldpc[nbch];
            }


            for (int j = 1; j < (plen - Xp); j++) {
              p[j] ^= p[j - 1];
            }

            if (signal_constellation == MOD_128APSK) {
              for (int j = 0; j < 6; j++) {
                p[j + plen] = 0;
              }
            }
            d += nbch;
            p += frame_size;
          }

          free(in_ldpc);
        }

        consume_each ((int) noutput_items/frame_size*kbch);
        return noutput_items;
    }


    const int dvb_bch_ldpc_bb_impl::ldpc_tab_1_4N[45][13]=
    {
      {12,23606,36098,1140,28859,18148,18510,6226,540,42014,20879,23802,47088},
      {12,16419,24928,16609,17248,7693,24997,42587,16858,34921,21042,37024,20692},
      {12,1874,40094,18704,14474,14004,11519,13106,28826,38669,22363,30255,31105},
      {12,22254,40564,22645,22532,6134,9176,39998,23892,8937,15608,16854,31009},
      {12,8037,40401,13550,19526,41902,28782,13304,32796,24679,27140,45980,10021},
      {12,40540,44498,13911,22435,32701,18405,39929,25521,12497,9851,39223,34823},
      {12,15233,45333,5041,44979,45710,42150,19416,1892,23121,15860,8832,10308},
      {12,10468,44296,3611,1480,37581,32254,13817,6883,32892,40258,46538,11940},
      {12,6705,21634,28150,43757,895,6547,20970,28914,30117,25736,41734,11392},
      {12,22002,5739,27210,27828,34192,37992,10915,6998,3824,42130,4494,35739},
      {12,8515,1191,13642,30950,25943,12673,16726,34261,31828,3340,8747,39225},
      {12,18979,17058,43130,4246,4793,44030,19454,29511,47929,15174,24333,19354},
      {12,16694,8381,29642,46516,32224,26344,9405,18292,12437,27316,35466,41992},
      {12,15642,5871,46489,26723,23396,7257,8974,3156,37420,44823,35423,13541},
      {12,42858,32008,41282,38773,26570,2702,27260,46974,1469,20887,27426,38553},
      {3,22152,24261,8297,0,0,0,0,0,0,0,0,0},
      {3,19347,9978,27802,0,0,0,0,0,0,0,0,0},
      {3,34991,6354,33561,0,0,0,0,0,0,0,0,0},
      {3,29782,30875,29523,0,0,0,0,0,0,0,0,0},
      {3,9278,48512,14349,0,0,0,0,0,0,0,0,0},
      {3,38061,4165,43878,0,0,0,0,0,0,0,0,0},
      {3,8548,33172,34410,0,0,0,0,0,0,0,0,0},
      {3,22535,28811,23950,0,0,0,0,0,0,0,0,0},
      {3,20439,4027,24186,0,0,0,0,0,0,0,0,0},
      {3,38618,8187,30947,0,0,0,0,0,0,0,0,0},
      {3,35538,43880,21459,0,0,0,0,0,0,0,0,0},
      {3,7091,45616,15063,0,0,0,0,0,0,0,0,0},
      {3,5505,9315,21908,0,0,0,0,0,0,0,0,0},
      {3,36046,32914,11836,0,0,0,0,0,0,0,0,0},
      {3,7304,39782,33721,0,0,0,0,0,0,0,0,0},
      {3,16905,29962,12980,0,0,0,0,0,0,0,0,0},
      {3,11171,23709,22460,0,0,0,0,0,0,0,0,0},
      {3,34541,9937,44500,0,0,0,0,0,0,0,0,0},
      {3,14035,47316,8815,0,0,0,0,0,0,0,0,0},
      {3,15057,45482,24461,0,0,0,0,0,0,0,0,0},
      {3,30518,36877,879,0,0,0,0,0,0,0,0,0},
      {3,7583,13364,24332,0,0,0,0,0,0,0,0,0},
      {3,448,27056,4682,0,0,0,0,0,0,0,0,0},
      {3,12083,31378,21670,0,0,0,0,0,0,0,0,0},
      {3,1159,18031,2221,0,0,0,0,0,0,0,0,0},
      {3,17028,38715,9350,0,0,0,0,0,0,0,0,0},
      {3,17343,24530,29574,0,0,0,0,0,0,0,0,0},
      {3,46128,31039,32818,0,0,0,0,0,0,0,0,0},
      {3,20373,36967,18345,0,0,0,0,0,0,0,0,0},
      {3,46685,20622,32806,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_1_3N[60][13]=
    {
      {12,34903,20927,32093,1052,25611,16093,16454,5520,506,37399,18518,21120},
      {12,11636,14594,22158,14763,15333,6838,22222,37856,14985,31041,18704,32910},
      {12,17449,1665,35639,16624,12867,12449,10241,11650,25622,34372,19878,26894},
      {12,29235,19780,36056,20129,20029,5457,8157,35554,21237,7943,13873,14980},
      {12,9912,7143,35911,12043,17360,37253,25588,11827,29152,21936,24125,40870},
      {12,40701,36035,39556,12366,19946,29072,16365,35495,22686,11106,8756,34863},
      {12,19165,15702,13536,40238,4465,40034,40590,37540,17162,1712,20577,14138},
      {12,31338,19342,9301,39375,3211,1316,33409,28670,12282,6118,29236,35787},
      {12,11504,30506,19558,5100,24188,24738,30397,33775,9699,6215,3397,37451},
      {12,34689,23126,7571,1058,12127,27518,23064,11265,14867,30451,28289,2966},
      {12,11660,15334,16867,15160,38343,3778,4265,39139,17293,26229,42604,13486},
      {12,31497,1365,14828,7453,26350,41346,28643,23421,8354,16255,11055,24279},
      {12,15687,12467,13906,5215,41328,23755,20800,6447,7970,2803,33262,39843},
      {12,5363,22469,38091,28457,36696,34471,23619,2404,24229,41754,1297,18563},
      {12,3673,39070,14480,30279,37483,7580,29519,30519,39831,20252,18132,20010},
      {12,34386,7252,27526,12950,6875,43020,31566,39069,18985,15541,40020,16715},
      {12,1721,37332,39953,17430,32134,29162,10490,12971,28581,29331,6489,35383},
      {12,736,7022,42349,8783,6767,11871,21675,10325,11548,25978,431,24085},
      {12,1925,10602,28585,12170,15156,34404,8351,13273,20208,5800,15367,21764},
      {12,16279,37832,34792,21250,34192,7406,41488,18346,29227,26127,25493,7048},
      {3,39948,28229,24899,0,0,0,0,0,0,0,0,0},
      {3,17408,14274,38993,0,0,0,0,0,0,0,0,0},
      {3,38774,15968,28459,0,0,0,0,0,0,0,0,0},
      {3,41404,27249,27425,0,0,0,0,0,0,0,0,0},
      {3,41229,6082,43114,0,0,0,0,0,0,0,0,0},
      {3,13957,4979,40654,0,0,0,0,0,0,0,0,0},
      {3,3093,3438,34992,0,0,0,0,0,0,0,0,0},
      {3,34082,6172,28760,0,0,0,0,0,0,0,0,0},
      {3,42210,34141,41021,0,0,0,0,0,0,0,0,0},
      {3,14705,17783,10134,0,0,0,0,0,0,0,0,0},
      {3,41755,39884,22773,0,0,0,0,0,0,0,0,0},
      {3,14615,15593,1642,0,0,0,0,0,0,0,0,0},
      {3,29111,37061,39860,0,0,0,0,0,0,0,0,0},
      {3,9579,33552,633,0,0,0,0,0,0,0,0,0},
      {3,12951,21137,39608,0,0,0,0,0,0,0,0,0},
      {3,38244,27361,29417,0,0,0,0,0,0,0,0,0},
      {3,2939,10172,36479,0,0,0,0,0,0,0,0,0},
      {3,29094,5357,19224,0,0,0,0,0,0,0,0,0},
      {3,9562,24436,28637,0,0,0,0,0,0,0,0,0},
      {3,40177,2326,13504,0,0,0,0,0,0,0,0,0},
      {3,6834,21583,42516,0,0,0,0,0,0,0,0,0},
      {3,40651,42810,25709,0,0,0,0,0,0,0,0,0},
      {3,31557,32138,38142,0,0,0,0,0,0,0,0,0},
      {3,18624,41867,39296,0,0,0,0,0,0,0,0,0},
      {3,37560,14295,16245,0,0,0,0,0,0,0,0,0},
      {3,6821,21679,31570,0,0,0,0,0,0,0,0,0},
      {3,25339,25083,22081,0,0,0,0,0,0,0,0,0},
      {3,8047,697,35268,0,0,0,0,0,0,0,0,0},
      {3,9884,17073,19995,0,0,0,0,0,0,0,0,0},
      {3,26848,35245,8390,0,0,0,0,0,0,0,0,0},
      {3,18658,16134,14807,0,0,0,0,0,0,0,0,0},
      {3,12201,32944,5035,0,0,0,0,0,0,0,0,0},
      {3,25236,1216,38986,0,0,0,0,0,0,0,0,0},
      {3,42994,24782,8681,0,0,0,0,0,0,0,0,0},
      {3,28321,4932,34249,0,0,0,0,0,0,0,0,0},
      {3,4107,29382,32124,0,0,0,0,0,0,0,0,0},
      {3,22157,2624,14468,0,0,0,0,0,0,0,0,0},
      {3,38788,27081,7936,0,0,0,0,0,0,0,0,0},
      {3,4368,26148,10578,0,0,0,0,0,0,0,0,0},
      {3,25353,4122,39751,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_2_5N[72][13]=
    {
      {12,31413,18834,28884,947,23050,14484,14809,4968,455,33659,16666,19008},
      {12,13172,19939,13354,13719,6132,20086,34040,13442,27958,16813,29619,16553},
      {12,1499,32075,14962,11578,11204,9217,10485,23062,30936,17892,24204,24885},
      {12,32490,18086,18007,4957,7285,32073,19038,7152,12486,13483,24808,21759},
      {12,32321,10839,15620,33521,23030,10646,26236,19744,21713,36784,8016,12869},
      {12,35597,11129,17948,26160,14729,31943,20416,10000,7882,31380,27858,33356},
      {12,14125,12131,36199,4058,35992,36594,33698,15475,1566,18498,12725,7067},
      {12,17406,8372,35437,2888,1184,30068,25802,11056,5507,26313,32205,37232},
      {12,15254,5365,17308,22519,35009,718,5240,16778,23131,24092,20587,33385},
      {12,27455,17602,4590,21767,22266,27357,30400,8732,5596,3060,33703,3596},
      {12,6882,873,10997,24738,20770,10067,13379,27409,25463,2673,6998,31378},
      {12,15181,13645,34501,3393,3840,35227,15562,23615,38342,12139,19471,15483},
      {12,13350,6707,23709,37204,25778,21082,7511,14588,10010,21854,28375,33591},
      {12,12514,4695,37190,21379,18723,5802,7182,2529,29936,35860,28338,10835},
      {12,34283,25610,33026,31017,21259,2165,21807,37578,1175,16710,21939,30841},
      {12,27292,33730,6836,26476,27539,35784,18245,16394,17939,23094,19216,17432},
      {12,11655,6183,38708,28408,35157,17089,13998,36029,15052,16617,5638,36464},
      {12,15693,28923,26245,9432,11675,25720,26405,5838,31851,26898,8090,37037},
      {12,24418,27583,7959,35562,37771,17784,11382,11156,37855,7073,21685,34515},
      {12,10977,13633,30969,7516,11943,18199,5231,13825,19589,23661,11150,35602},
      {12,19124,30774,6670,37344,16510,26317,23518,22957,6348,34069,8845,20175},
      {12,34985,14441,25668,4116,3019,21049,37308,24551,24727,20104,24850,12114},
      {12,38187,28527,13108,13985,1425,21477,30807,8613,26241,33368,35913,32477},
      {12,5903,34390,24641,26556,23007,27305,38247,2621,9122,32806,21554,18685},
      {3,17287,27292,19033,0,0,0,0,0,0,0,0,0},
      {3,25796,31795,12152,0,0,0,0,0,0,0,0,0},
      {3,12184,35088,31226,0,0,0,0,0,0,0,0,0},
      {3,38263,33386,24892,0,0,0,0,0,0,0,0,0},
      {3,23114,37995,29796,0,0,0,0,0,0,0,0,0},
      {3,34336,10551,36245,0,0,0,0,0,0,0,0,0},
      {3,35407,175,7203,0,0,0,0,0,0,0,0,0},
      {3,14654,38201,22605,0,0,0,0,0,0,0,0,0},
      {3,28404,6595,1018,0,0,0,0,0,0,0,0,0},
      {3,19932,3524,29305,0,0,0,0,0,0,0,0,0},
      {3,31749,20247,8128,0,0,0,0,0,0,0,0,0},
      {3,18026,36357,26735,0,0,0,0,0,0,0,0,0},
      {3,7543,29767,13588,0,0,0,0,0,0,0,0,0},
      {3,13333,25965,8463,0,0,0,0,0,0,0,0,0},
      {3,14504,36796,19710,0,0,0,0,0,0,0,0,0},
      {3,4528,25299,7318,0,0,0,0,0,0,0,0,0},
      {3,35091,25550,14798,0,0,0,0,0,0,0,0,0},
      {3,7824,215,1248,0,0,0,0,0,0,0,0,0},
      {3,30848,5362,17291,0,0,0,0,0,0,0,0,0},
      {3,28932,30249,27073,0,0,0,0,0,0,0,0,0},
      {3,13062,2103,16206,0,0,0,0,0,0,0,0,0},
      {3,7129,32062,19612,0,0,0,0,0,0,0,0,0},
      {3,9512,21936,38833,0,0,0,0,0,0,0,0,0},
      {3,35849,33754,23450,0,0,0,0,0,0,0,0,0},
      {3,18705,28656,18111,0,0,0,0,0,0,0,0,0},
      {3,22749,27456,32187,0,0,0,0,0,0,0,0,0},
      {3,28229,31684,30160,0,0,0,0,0,0,0,0,0},
      {3,15293,8483,28002,0,0,0,0,0,0,0,0,0},
      {3,14880,13334,12584,0,0,0,0,0,0,0,0,0},
      {3,28646,2558,19687,0,0,0,0,0,0,0,0,0},
      {3,6259,4499,26336,0,0,0,0,0,0,0,0,0},
      {3,11952,28386,8405,0,0,0,0,0,0,0,0,0},
      {3,10609,961,7582,0,0,0,0,0,0,0,0,0},
      {3,10423,13191,26818,0,0,0,0,0,0,0,0,0},
      {3,15922,36654,21450,0,0,0,0,0,0,0,0,0},
      {3,10492,1532,1205,0,0,0,0,0,0,0,0,0},
      {3,30551,36482,22153,0,0,0,0,0,0,0,0,0},
      {3,5156,11330,34243,0,0,0,0,0,0,0,0,0},
      {3,28616,35369,13322,0,0,0,0,0,0,0,0,0},
      {3,8962,1485,21186,0,0,0,0,0,0,0,0,0},
      {3,23541,17445,35561,0,0,0,0,0,0,0,0,0},
      {3,33133,11593,19895,0,0,0,0,0,0,0,0,0},
      {3,33917,7863,33651,0,0,0,0,0,0,0,0,0},
      {3,20063,28331,10702,0,0,0,0,0,0,0,0,0},
      {3,13195,21107,21859,0,0,0,0,0,0,0,0,0},
      {3,4364,31137,4804,0,0,0,0,0,0,0,0,0},
      {3,5585,2037,4830,0,0,0,0,0,0,0,0,0},
      {3,30672,16927,14800,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_1_2N[90][9]=
    {
      {8,54,9318,14392,27561,26909,10219,2534,8597},
      {8,55,7263,4635,2530,28130,3033,23830,3651},
      {8,56,24731,23583,26036,17299,5750,792,9169},
      {8,57,5811,26154,18653,11551,15447,13685,16264},
      {8,58,12610,11347,28768,2792,3174,29371,12997},
      {8,59,16789,16018,21449,6165,21202,15850,3186},
      {8,60,31016,21449,17618,6213,12166,8334,18212},
      {8,61,22836,14213,11327,5896,718,11727,9308},
      {8,62,2091,24941,29966,23634,9013,15587,5444},
      {8,63,22207,3983,16904,28534,21415,27524,25912},
      {8,64,25687,4501,22193,14665,14798,16158,5491},
      {8,65,4520,17094,23397,4264,22370,16941,21526},
      {8,66,10490,6182,32370,9597,30841,25954,2762},
      {8,67,22120,22865,29870,15147,13668,14955,19235},
      {8,68,6689,18408,18346,9918,25746,5443,20645},
      {8,69,29982,12529,13858,4746,30370,10023,24828},
      {8,70,1262,28032,29888,13063,24033,21951,7863},
      {8,71,6594,29642,31451,14831,9509,9335,31552},
      {8,72,1358,6454,16633,20354,24598,624,5265},
      {8,73,19529,295,18011,3080,13364,8032,15323},
      {8,74,11981,1510,7960,21462,9129,11370,25741},
      {8,75,9276,29656,4543,30699,20646,21921,28050},
      {8,76,15975,25634,5520,31119,13715,21949,19605},
      {8,77,18688,4608,31755,30165,13103,10706,29224},
      {8,78,21514,23117,12245,26035,31656,25631,30699},
      {8,79,9674,24966,31285,29908,17042,24588,31857},
      {8,80,21856,27777,29919,27000,14897,11409,7122},
      {8,81,29773,23310,263,4877,28622,20545,22092},
      {8,82,15605,5651,21864,3967,14419,22757,15896},
      {8,83,30145,1759,10139,29223,26086,10556,5098},
      {8,84,18815,16575,2936,24457,26738,6030,505},
      {8,85,30326,22298,27562,20131,26390,6247,24791},
      {8,86,928,29246,21246,12400,15311,32309,18608},
      {8,87,20314,6025,26689,16302,2296,3244,19613},
      {8,88,6237,11943,22851,15642,23857,15112,20947},
      {8,89,26403,25168,19038,18384,8882,12719,7093},
      {3,0,14567,24965,0,0,0,0,0},
      {3,1,3908,100,0,0,0,0,0},
      {3,2,10279,240,0,0,0,0,0},
      {3,3,24102,764,0,0,0,0,0},
      {3,4,12383,4173,0,0,0,0,0},
      {3,5,13861,15918,0,0,0,0,0},
      {3,6,21327,1046,0,0,0,0,0},
      {3,7,5288,14579,0,0,0,0,0},
      {3,8,28158,8069,0,0,0,0,0},
      {3,9,16583,11098,0,0,0,0,0},
      {3,10,16681,28363,0,0,0,0,0},
      {3,11,13980,24725,0,0,0,0,0},
      {3,12,32169,17989,0,0,0,0,0},
      {3,13,10907,2767,0,0,0,0,0},
      {3,14,21557,3818,0,0,0,0,0},
      {3,15,26676,12422,0,0,0,0,0},
      {3,16,7676,8754,0,0,0,0,0},
      {3,17,14905,20232,0,0,0,0,0},
      {3,18,15719,24646,0,0,0,0,0},
      {3,19,31942,8589,0,0,0,0,0},
      {3,20,19978,27197,0,0,0,0,0},
      {3,21,27060,15071,0,0,0,0,0},
      {3,22,6071,26649,0,0,0,0,0},
      {3,23,10393,11176,0,0,0,0,0},
      {3,24,9597,13370,0,0,0,0,0},
      {3,25,7081,17677,0,0,0,0,0},
      {3,26,1433,19513,0,0,0,0,0},
      {3,27,26925,9014,0,0,0,0,0},
      {3,28,19202,8900,0,0,0,0,0},
      {3,29,18152,30647,0,0,0,0,0},
      {3,30,20803,1737,0,0,0,0,0},
      {3,31,11804,25221,0,0,0,0,0},
      {3,32,31683,17783,0,0,0,0,0},
      {3,33,29694,9345,0,0,0,0,0},
      {3,34,12280,26611,0,0,0,0,0},
      {3,35,6526,26122,0,0,0,0,0},
      {3,36,26165,11241,0,0,0,0,0},
      {3,37,7666,26962,0,0,0,0,0},
      {3,38,16290,8480,0,0,0,0,0},
      {3,39,11774,10120,0,0,0,0,0},
      {3,40,30051,30426,0,0,0,0,0},
      {3,41,1335,15424,0,0,0,0,0},
      {3,42,6865,17742,0,0,0,0,0},
      {3,43,31779,12489,0,0,0,0,0},
      {3,44,32120,21001,0,0,0,0,0},
      {3,45,14508,6996,0,0,0,0,0},
      {3,46,979,25024,0,0,0,0,0},
      {3,47,4554,21896,0,0,0,0,0},
      {3,48,7989,21777,0,0,0,0,0},
      {3,49,4972,20661,0,0,0,0,0},
      {3,50,6612,2730,0,0,0,0,0},
      {3,51,12742,4418,0,0,0,0,0},
      {3,52,29194,595,0,0,0,0,0},
      {3,53,19267,20113,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_3_5N[108][13]=
    {
      {12,22422,10282,11626,19997,11161,2922,3122,99,5625,17064,8270,179},
      {12,25087,16218,17015,828,20041,25656,4186,11629,22599,17305,22515,6463},
      {12,11049,22853,25706,14388,5500,19245,8732,2177,13555,11346,17265,3069},
      {12,16581,22225,12563,19717,23577,11555,25496,6853,25403,5218,15925,21766},
      {12,16529,14487,7643,10715,17442,11119,5679,14155,24213,21000,1116,15620},
      {12,5340,8636,16693,1434,5635,6516,9482,20189,1066,15013,25361,14243},
      {12,18506,22236,20912,8952,5421,15691,6126,21595,500,6904,13059,6802},
      {12,8433,4694,5524,14216,3685,19721,25420,9937,23813,9047,25651,16826},
      {12,21500,24814,6344,17382,7064,13929,4004,16552,12818,8720,5286,2206},
      {12,22517,2429,19065,2921,21611,1873,7507,5661,23006,23128,20543,19777},
      {12,1770,4636,20900,14931,9247,12340,11008,12966,4471,2731,16445,791},
      {12,6635,14556,18865,22421,22124,12697,9803,25485,7744,18254,11313,9004},
      {12,19982,23963,18912,7206,12500,4382,20067,6177,21007,1195,23547,24837},
      {12,756,11158,14646,20534,3647,17728,11676,11843,12937,4402,8261,22944},
      {12,9306,24009,10012,11081,3746,24325,8060,19826,842,8836,2898,5019},
      {12,7575,7455,25244,4736,14400,22981,5543,8006,24203,13053,1120,5128},
      {12,3482,9270,13059,15825,7453,23747,3656,24585,16542,17507,22462,14670},
      {12,15627,15290,4198,22748,5842,13395,23918,16985,14929,3726,25350,24157},
      {12,24896,16365,16423,13461,16615,8107,24741,3604,25904,8716,9604,20365},
      {12,3729,17245,18448,9862,20831,25326,20517,24618,13282,5099,14183,8804},
      {12,16455,17646,15376,18194,25528,1777,6066,21855,14372,12517,4488,17490},
      {12,1400,8135,23375,20879,8476,4084,12936,25536,22309,16582,6402,24360},
      {12,25119,23586,128,4761,10443,22536,8607,9752,25446,15053,1856,4040},
      {12,377,21160,13474,5451,17170,5938,10256,11972,24210,17833,22047,16108},
      {12,13075,9648,24546,13150,23867,7309,19798,2988,16858,4825,23950,15125},
      {12,20526,3553,11525,23366,2452,17626,19265,20172,18060,24593,13255,1552},
      {12,18839,21132,20119,15214,14705,7096,10174,5663,18651,19700,12524,14033},
      {12,4127,2971,17499,16287,22368,21463,7943,18880,5567,8047,23363,6797},
      {12,10651,24471,14325,4081,7258,4949,7044,1078,797,22910,20474,4318},
      {12,21374,13231,22985,5056,3821,23718,14178,9978,19030,23594,8895,25358},
      {12,6199,22056,7749,13310,3999,23697,16445,22636,5225,22437,24153,9442},
      {12,7978,12177,2893,20778,3175,8645,11863,24623,10311,25767,17057,3691},
      {12,20473,11294,9914,22815,2574,8439,3699,5431,24840,21908,16088,18244},
      {12,8208,5755,19059,8541,24924,6454,11234,10492,16406,10831,11436,9649},
      {12,16264,11275,24953,2347,12667,19190,7257,7174,24819,2938,2522,11749},
      {12,3627,5969,13862,1538,23176,6353,2855,17720,2472,7428,573,15036},
      {3,0,18539,18661,0,0,0,0,0,0,0,0,0},
      {3,1,10502,3002,0,0,0,0,0,0,0,0,0},
      {3,2,9368,10761,0,0,0,0,0,0,0,0,0},
      {3,3,12299,7828,0,0,0,0,0,0,0,0,0},
      {3,4,15048,13362,0,0,0,0,0,0,0,0,0},
      {3,5,18444,24640,0,0,0,0,0,0,0,0,0},
      {3,6,20775,19175,0,0,0,0,0,0,0,0,0},
      {3,7,18970,10971,0,0,0,0,0,0,0,0,0},
      {3,8,5329,19982,0,0,0,0,0,0,0,0,0},
      {3,9,11296,18655,0,0,0,0,0,0,0,0,0},
      {3,10,15046,20659,0,0,0,0,0,0,0,0,0},
      {3,11,7300,22140,0,0,0,0,0,0,0,0,0},
      {3,12,22029,14477,0,0,0,0,0,0,0,0,0},
      {3,13,11129,742,0,0,0,0,0,0,0,0,0},
      {3,14,13254,13813,0,0,0,0,0,0,0,0,0},
      {3,15,19234,13273,0,0,0,0,0,0,0,0,0},
      {3,16,6079,21122,0,0,0,0,0,0,0,0,0},
      {3,17,22782,5828,0,0,0,0,0,0,0,0,0},
      {3,18,19775,4247,0,0,0,0,0,0,0,0,0},
      {3,19,1660,19413,0,0,0,0,0,0,0,0,0},
      {3,20,4403,3649,0,0,0,0,0,0,0,0,0},
      {3,21,13371,25851,0,0,0,0,0,0,0,0,0},
      {3,22,22770,21784,0,0,0,0,0,0,0,0,0},
      {3,23,10757,14131,0,0,0,0,0,0,0,0,0},
      {3,24,16071,21617,0,0,0,0,0,0,0,0,0},
      {3,25,6393,3725,0,0,0,0,0,0,0,0,0},
      {3,26,597,19968,0,0,0,0,0,0,0,0,0},
      {3,27,5743,8084,0,0,0,0,0,0,0,0,0},
      {3,28,6770,9548,0,0,0,0,0,0,0,0,0},
      {3,29,4285,17542,0,0,0,0,0,0,0,0,0},
      {3,30,13568,22599,0,0,0,0,0,0,0,0,0},
      {3,31,1786,4617,0,0,0,0,0,0,0,0,0},
      {3,32,23238,11648,0,0,0,0,0,0,0,0,0},
      {3,33,19627,2030,0,0,0,0,0,0,0,0,0},
      {3,34,13601,13458,0,0,0,0,0,0,0,0,0},
      {3,35,13740,17328,0,0,0,0,0,0,0,0,0},
      {3,36,25012,13944,0,0,0,0,0,0,0,0,0},
      {3,37,22513,6687,0,0,0,0,0,0,0,0,0},
      {3,38,4934,12587,0,0,0,0,0,0,0,0,0},
      {3,39,21197,5133,0,0,0,0,0,0,0,0,0},
      {3,40,22705,6938,0,0,0,0,0,0,0,0,0},
      {3,41,7534,24633,0,0,0,0,0,0,0,0,0},
      {3,42,24400,12797,0,0,0,0,0,0,0,0,0},
      {3,43,21911,25712,0,0,0,0,0,0,0,0,0},
      {3,44,12039,1140,0,0,0,0,0,0,0,0,0},
      {3,45,24306,1021,0,0,0,0,0,0,0,0,0},
      {3,46,14012,20747,0,0,0,0,0,0,0,0,0},
      {3,47,11265,15219,0,0,0,0,0,0,0,0,0},
      {3,48,4670,15531,0,0,0,0,0,0,0,0,0},
      {3,49,9417,14359,0,0,0,0,0,0,0,0,0},
      {3,50,2415,6504,0,0,0,0,0,0,0,0,0},
      {3,51,24964,24690,0,0,0,0,0,0,0,0,0},
      {3,52,14443,8816,0,0,0,0,0,0,0,0,0},
      {3,53,6926,1291,0,0,0,0,0,0,0,0,0},
      {3,54,6209,20806,0,0,0,0,0,0,0,0,0},
      {3,55,13915,4079,0,0,0,0,0,0,0,0,0},
      {3,56,24410,13196,0,0,0,0,0,0,0,0,0},
      {3,57,13505,6117,0,0,0,0,0,0,0,0,0},
      {3,58,9869,8220,0,0,0,0,0,0,0,0,0},
      {3,59,1570,6044,0,0,0,0,0,0,0,0,0},
      {3,60,25780,17387,0,0,0,0,0,0,0,0,0},
      {3,61,20671,24913,0,0,0,0,0,0,0,0,0},
      {3,62,24558,20591,0,0,0,0,0,0,0,0,0},
      {3,63,12402,3702,0,0,0,0,0,0,0,0,0},
      {3,64,8314,1357,0,0,0,0,0,0,0,0,0},
      {3,65,20071,14616,0,0,0,0,0,0,0,0,0},
      {3,66,17014,3688,0,0,0,0,0,0,0,0,0},
      {3,67,19837,946,0,0,0,0,0,0,0,0,0},
      {3,68,15195,12136,0,0,0,0,0,0,0,0,0},
      {3,69,7758,22808,0,0,0,0,0,0,0,0,0},
      {3,70,3564,2925,0,0,0,0,0,0,0,0,0},
      {3,71,3434,7769,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_2_3N_DVBT2[120][14]=
    {
      {13,317,2255,2324,2723,3538,3576,6194,6700,9101,10057,12739,17407,21039},
      {13,1958,2007,3294,4394,12762,14505,14593,14692,16522,17737,19245,21272,21379},
      {13,127,860,5001,5633,8644,9282,12690,14644,17553,19511,19681,20954,21002},
      {13,2514,2822,5781,6297,8063,9469,9551,11407,11837,12985,15710,20236,20393},
      {13,1565,3106,4659,4926,6495,6872,7343,8720,15785,16434,16727,19884,21325},
      {13,706,3220,8568,10896,12486,13663,16398,16599,19475,19781,20625,20961,21335},
      {13,4257,10449,12406,14561,16049,16522,17214,18029,18033,18802,19062,19526,20748},
      {13,412,433,558,2614,2978,4157,6584,9320,11683,11819,13024,14486,16860},
      {13,777,5906,7403,8550,8717,8770,11436,12846,13629,14755,15688,16392,16419},
      {13,4093,5045,6037,7248,8633,9771,10260,10809,11326,12072,17516,19344,19938},
      {13,2120,2648,3155,3852,6888,12258,14821,15359,16378,16437,17791,20614,21025},
      {13,1085,2434,5816,7151,8050,9422,10884,12728,15353,17733,18140,18729,20920},
      {3,856,1690,12787,0,0,0,0,0,0,0,0,0},
      {3,6532,7357,9151,0,0,0,0,0,0,0,0,0},
      {3,4210,16615,18152,0,0,0,0,0,0,0,0,0},
      {3,11494,14036,17470,0,0,0,0,0,0,0,0,0},
      {3,2474,10291,10323,0,0,0,0,0,0,0,0,0},
      {3,1778,6973,10739,0,0,0,0,0,0,0,0,0},
      {3,4347,9570,18748,0,0,0,0,0,0,0,0,0},
      {3,2189,11942,20666,0,0,0,0,0,0,0,0,0},
      {3,3868,7526,17706,0,0,0,0,0,0,0,0,0},
      {3,8780,14796,18268,0,0,0,0,0,0,0,0,0},
      {3,160,16232,17399,0,0,0,0,0,0,0,0,0},
      {3,1285,2003,18922,0,0,0,0,0,0,0,0,0},
      {3,4658,17331,20361,0,0,0,0,0,0,0,0,0},
      {3,2765,4862,5875,0,0,0,0,0,0,0,0,0},
      {3,4565,5521,8759,0,0,0,0,0,0,0,0,0},
      {3,3484,7305,15829,0,0,0,0,0,0,0,0,0},
      {3,5024,17730,17879,0,0,0,0,0,0,0,0,0},
      {3,7031,12346,15024,0,0,0,0,0,0,0,0,0},
      {3,179,6365,11352,0,0,0,0,0,0,0,0,0},
      {3,2490,3143,5098,0,0,0,0,0,0,0,0,0},
      {3,2643,3101,21259,0,0,0,0,0,0,0,0,0},
      {3,4315,4724,13130,0,0,0,0,0,0,0,0,0},
      {3,594,17365,18322,0,0,0,0,0,0,0,0,0},
      {3,5983,8597,9627,0,0,0,0,0,0,0,0,0},
      {3,10837,15102,20876,0,0,0,0,0,0,0,0,0},
      {3,10448,20418,21478,0,0,0,0,0,0,0,0,0},
      {3,3848,12029,15228,0,0,0,0,0,0,0,0,0},
      {3,708,5652,13146,0,0,0,0,0,0,0,0,0},
      {3,5998,7534,16117,0,0,0,0,0,0,0,0,0},
      {3,2098,13201,18317,0,0,0,0,0,0,0,0,0},
      {3,9186,14548,17776,0,0,0,0,0,0,0,0,0},
      {3,5246,10398,18597,0,0,0,0,0,0,0,0,0},
      {3,3083,4944,21021,0,0,0,0,0,0,0,0,0},
      {3,13726,18495,19921,0,0,0,0,0,0,0,0,0},
      {3,6736,10811,17545,0,0,0,0,0,0,0,0,0},
      {3,10084,12411,14432,0,0,0,0,0,0,0,0,0},
      {3,1064,13555,17033,0,0,0,0,0,0,0,0,0},
      {3,679,9878,13547,0,0,0,0,0,0,0,0,0},
      {3,3422,9910,20194,0,0,0,0,0,0,0,0,0},
      {3,3640,3701,10046,0,0,0,0,0,0,0,0,0},
      {3,5862,10134,11498,0,0,0,0,0,0,0,0,0},
      {3,5923,9580,15060,0,0,0,0,0,0,0,0,0},
      {3,1073,3012,16427,0,0,0,0,0,0,0,0,0},
      {3,5527,20113,20883,0,0,0,0,0,0,0,0,0},
      {3,7058,12924,15151,0,0,0,0,0,0,0,0,0},
      {3,9764,12230,17375,0,0,0,0,0,0,0,0,0},
      {3,772,7711,12723,0,0,0,0,0,0,0,0,0},
      {3,555,13816,15376,0,0,0,0,0,0,0,0,0},
      {3,10574,11268,17932,0,0,0,0,0,0,0,0,0},
      {3,15442,17266,20482,0,0,0,0,0,0,0,0,0},
      {3,390,3371,8781,0,0,0,0,0,0,0,0,0},
      {3,10512,12216,17180,0,0,0,0,0,0,0,0,0},
      {3,4309,14068,15783,0,0,0,0,0,0,0,0,0},
      {3,3971,11673,20009,0,0,0,0,0,0,0,0,0},
      {3,9259,14270,17199,0,0,0,0,0,0,0,0,0},
      {3,2947,5852,20101,0,0,0,0,0,0,0,0,0},
      {3,3965,9722,15363,0,0,0,0,0,0,0,0,0},
      {3,1429,5689,16771,0,0,0,0,0,0,0,0,0},
      {3,6101,6849,12781,0,0,0,0,0,0,0,0,0},
      {3,3676,9347,18761,0,0,0,0,0,0,0,0,0},
      {3,350,11659,18342,0,0,0,0,0,0,0,0,0},
      {3,5961,14803,16123,0,0,0,0,0,0,0,0,0},
      {3,2113,9163,13443,0,0,0,0,0,0,0,0,0},
      {3,2155,9808,12885,0,0,0,0,0,0,0,0,0},
      {3,2861,7988,11031,0,0,0,0,0,0,0,0,0},
      {3,7309,9220,20745,0,0,0,0,0,0,0,0,0},
      {3,6834,8742,11977,0,0,0,0,0,0,0,0,0},
      {3,2133,12908,14704,0,0,0,0,0,0,0,0,0},
      {3,10170,13809,18153,0,0,0,0,0,0,0,0,0},
      {3,13464,14787,14975,0,0,0,0,0,0,0,0,0},
      {3,799,1107,3789,0,0,0,0,0,0,0,0,0},
      {3,3571,8176,10165,0,0,0,0,0,0,0,0,0},
      {3,5433,13446,15481,0,0,0,0,0,0,0,0,0},
      {3,3351,6767,12840,0,0,0,0,0,0,0,0,0},
      {3,8950,8974,11650,0,0,0,0,0,0,0,0,0},
      {3,1430,4250,21332,0,0,0,0,0,0,0,0,0},
      {3,6283,10628,15050,0,0,0,0,0,0,0,0,0},
      {3,8632,14404,16916,0,0,0,0,0,0,0,0,0},
      {3,6509,10702,16278,0,0,0,0,0,0,0,0,0},
      {3,15900,16395,17995,0,0,0,0,0,0,0,0,0},
      {3,8031,18420,19733,0,0,0,0,0,0,0,0,0},
      {3,3747,4634,17087,0,0,0,0,0,0,0,0,0},
      {3,4453,6297,16262,0,0,0,0,0,0,0,0,0},
      {3,2792,3513,17031,0,0,0,0,0,0,0,0,0},
      {3,14846,20893,21563,0,0,0,0,0,0,0,0,0},
      {3,17220,20436,21337,0,0,0,0,0,0,0,0,0},
      {3,275,4107,10497,0,0,0,0,0,0,0,0,0},
      {3,3536,7520,10027,0,0,0,0,0,0,0,0,0},
      {3,14089,14943,19455,0,0,0,0,0,0,0,0,0},
      {3,1965,3931,21104,0,0,0,0,0,0,0,0,0},
      {3,2439,11565,17932,0,0,0,0,0,0,0,0,0},
      {3,154,15279,21414,0,0,0,0,0,0,0,0,0},
      {3,10017,11269,16546,0,0,0,0,0,0,0,0,0},
      {3,7169,10161,16928,0,0,0,0,0,0,0,0,0},
      {3,10284,16791,20655,0,0,0,0,0,0,0,0,0},
      {3,36,3175,8475,0,0,0,0,0,0,0,0,0},
      {3,2605,16269,19290,0,0,0,0,0,0,0,0,0},
      {3,8947,9178,15420,0,0,0,0,0,0,0,0,0},
      {3,5687,9156,12408,0,0,0,0,0,0,0,0,0},
      {3,8096,9738,14711,0,0,0,0,0,0,0,0,0},
      {3,4935,8093,19266,0,0,0,0,0,0,0,0,0},
      {3,2667,10062,15972,0,0,0,0,0,0,0,0,0},
      {3,6389,11318,14417,0,0,0,0,0,0,0,0,0},
      {3,8800,18137,18434,0,0,0,0,0,0,0,0,0},
      {3,5824,5927,15314,0,0,0,0,0,0,0,0,0},
      {3,6056,13168,15179,0,0,0,0,0,0,0,0,0},
      {3,3284,13138,18919,0,0,0,0,0,0,0,0,0},
      {3,13115,17259,17332,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_2_3N_DVBS2[120][14]=
    {
      {13,0,10491,16043,506,12826,8065,8226,2767,240,18673,9279,10579,20928},
      {13,1,17819,8313,6433,6224,5120,5824,12812,17187,9940,13447,13825,18483},
      {13,2,17957,6024,8681,18628,12794,5915,14576,10970,12064,20437,4455,7151},
      {13,3,19777,6183,9972,14536,8182,17749,11341,5556,4379,17434,15477,18532},
      {13,4,4651,19689,1608,659,16707,14335,6143,3058,14618,17894,20684,5306},
      {13,5,9778,2552,12096,12369,15198,16890,4851,3109,1700,18725,1997,15882},
      {13,6,486,6111,13743,11537,5591,7433,15227,14145,1483,3887,17431,12430},
      {13,7,20647,14311,11734,4180,8110,5525,12141,15761,18661,18441,10569,8192},
      {13,8,3791,14759,15264,19918,10132,9062,10010,12786,10675,9682,19246,5454},
      {13,9,19525,9485,7777,19999,8378,9209,3163,20232,6690,16518,716,7353},
      {13,10,4588,6709,20202,10905,915,4317,11073,13576,16433,368,3508,21171},
      {13,11,14072,4033,19959,12608,631,19494,14160,8249,10223,21504,12395,4322},
      {3,12,13800,14161,0,0,0,0,0,0,0,0,0,0},
      {3,13,2948,9647,0,0,0,0,0,0,0,0,0,0},
      {3,14,14693,16027,0,0,0,0,0,0,0,0,0,0},
      {3,15,20506,11082,0,0,0,0,0,0,0,0,0,0},
      {3,16,1143,9020,0,0,0,0,0,0,0,0,0,0},
      {3,17,13501,4014,0,0,0,0,0,0,0,0,0,0},
      {3,18,1548,2190,0,0,0,0,0,0,0,0,0,0},
      {3,19,12216,21556,0,0,0,0,0,0,0,0,0,0},
      {3,20,2095,19897,0,0,0,0,0,0,0,0,0,0},
      {3,21,4189,7958,0,0,0,0,0,0,0,0,0,0},
      {3,22,15940,10048,0,0,0,0,0,0,0,0,0,0},
      {3,23,515,12614,0,0,0,0,0,0,0,0,0,0},
      {3,24,8501,8450,0,0,0,0,0,0,0,0,0,0},
      {3,25,17595,16784,0,0,0,0,0,0,0,0,0,0},
      {3,26,5913,8495,0,0,0,0,0,0,0,0,0,0},
      {3,27,16394,10423,0,0,0,0,0,0,0,0,0,0},
      {3,28,7409,6981,0,0,0,0,0,0,0,0,0,0},
      {3,29,6678,15939,0,0,0,0,0,0,0,0,0,0},
      {3,30,20344,12987,0,0,0,0,0,0,0,0,0,0},
      {3,31,2510,14588,0,0,0,0,0,0,0,0,0,0},
      {3,32,17918,6655,0,0,0,0,0,0,0,0,0,0},
      {3,33,6703,19451,0,0,0,0,0,0,0,0,0,0},
      {3,34,496,4217,0,0,0,0,0,0,0,0,0,0},
      {3,35,7290,5766,0,0,0,0,0,0,0,0,0,0},
      {3,36,10521,8925,0,0,0,0,0,0,0,0,0,0},
      {3,37,20379,11905,0,0,0,0,0,0,0,0,0,0},
      {3,38,4090,5838,0,0,0,0,0,0,0,0,0,0},
      {3,39,19082,17040,0,0,0,0,0,0,0,0,0,0},
      {3,40,20233,12352,0,0,0,0,0,0,0,0,0,0},
      {3,41,19365,19546,0,0,0,0,0,0,0,0,0,0},
      {3,42,6249,19030,0,0,0,0,0,0,0,0,0,0},
      {3,43,11037,19193,0,0,0,0,0,0,0,0,0,0},
      {3,44,19760,11772,0,0,0,0,0,0,0,0,0,0},
      {3,45,19644,7428,0,0,0,0,0,0,0,0,0,0},
      {3,46,16076,3521,0,0,0,0,0,0,0,0,0,0},
      {3,47,11779,21062,0,0,0,0,0,0,0,0,0,0},
      {3,48,13062,9682,0,0,0,0,0,0,0,0,0,0},
      {3,49,8934,5217,0,0,0,0,0,0,0,0,0,0},
      {3,50,11087,3319,0,0,0,0,0,0,0,0,0,0},
      {3,51,18892,4356,0,0,0,0,0,0,0,0,0,0},
      {3,52,7894,3898,0,0,0,0,0,0,0,0,0,0},
      {3,53,5963,4360,0,0,0,0,0,0,0,0,0,0},
      {3,54,7346,11726,0,0,0,0,0,0,0,0,0,0},
      {3,55,5182,5609,0,0,0,0,0,0,0,0,0,0},
      {3,56,2412,17295,0,0,0,0,0,0,0,0,0,0},
      {3,57,9845,20494,0,0,0,0,0,0,0,0,0,0},
      {3,58,6687,1864,0,0,0,0,0,0,0,0,0,0},
      {3,59,20564,5216,0,0,0,0,0,0,0,0,0,0},
      {3,0,18226,17207,0,0,0,0,0,0,0,0,0,0},
      {3,1,9380,8266,0,0,0,0,0,0,0,0,0,0},
      {3,2,7073,3065,0,0,0,0,0,0,0,0,0,0},
      {3,3,18252,13437,0,0,0,0,0,0,0,0,0,0},
      {3,4,9161,15642,0,0,0,0,0,0,0,0,0,0},
      {3,5,10714,10153,0,0,0,0,0,0,0,0,0,0},
      {3,6,11585,9078,0,0,0,0,0,0,0,0,0,0},
      {3,7,5359,9418,0,0,0,0,0,0,0,0,0,0},
      {3,8,9024,9515,0,0,0,0,0,0,0,0,0,0},
      {3,9,1206,16354,0,0,0,0,0,0,0,0,0,0},
      {3,10,14994,1102,0,0,0,0,0,0,0,0,0,0},
      {3,11,9375,20796,0,0,0,0,0,0,0,0,0,0},
      {3,12,15964,6027,0,0,0,0,0,0,0,0,0,0},
      {3,13,14789,6452,0,0,0,0,0,0,0,0,0,0},
      {3,14,8002,18591,0,0,0,0,0,0,0,0,0,0},
      {3,15,14742,14089,0,0,0,0,0,0,0,0,0,0},
      {3,16,253,3045,0,0,0,0,0,0,0,0,0,0},
      {3,17,1274,19286,0,0,0,0,0,0,0,0,0,0},
      {3,18,14777,2044,0,0,0,0,0,0,0,0,0,0},
      {3,19,13920,9900,0,0,0,0,0,0,0,0,0,0},
      {3,20,452,7374,0,0,0,0,0,0,0,0,0,0},
      {3,21,18206,9921,0,0,0,0,0,0,0,0,0,0},
      {3,22,6131,5414,0,0,0,0,0,0,0,0,0,0},
      {3,23,10077,9726,0,0,0,0,0,0,0,0,0,0},
      {3,24,12045,5479,0,0,0,0,0,0,0,0,0,0},
      {3,25,4322,7990,0,0,0,0,0,0,0,0,0,0},
      {3,26,15616,5550,0,0,0,0,0,0,0,0,0,0},
      {3,27,15561,10661,0,0,0,0,0,0,0,0,0,0},
      {3,28,20718,7387,0,0,0,0,0,0,0,0,0,0},
      {3,29,2518,18804,0,0,0,0,0,0,0,0,0,0},
      {3,30,8984,2600,0,0,0,0,0,0,0,0,0,0},
      {3,31,6516,17909,0,0,0,0,0,0,0,0,0,0},
      {3,32,11148,98,0,0,0,0,0,0,0,0,0,0},
      {3,33,20559,3704,0,0,0,0,0,0,0,0,0,0},
      {3,34,7510,1569,0,0,0,0,0,0,0,0,0,0},
      {3,35,16000,11692,0,0,0,0,0,0,0,0,0,0},
      {3,36,9147,10303,0,0,0,0,0,0,0,0,0,0},
      {3,37,16650,191,0,0,0,0,0,0,0,0,0,0},
      {3,38,15577,18685,0,0,0,0,0,0,0,0,0,0},
      {3,39,17167,20917,0,0,0,0,0,0,0,0,0,0},
      {3,40,4256,3391,0,0,0,0,0,0,0,0,0,0},
      {3,41,20092,17219,0,0,0,0,0,0,0,0,0,0},
      {3,42,9218,5056,0,0,0,0,0,0,0,0,0,0},
      {3,43,18429,8472,0,0,0,0,0,0,0,0,0,0},
      {3,44,12093,20753,0,0,0,0,0,0,0,0,0,0},
      {3,45,16345,12748,0,0,0,0,0,0,0,0,0,0},
      {3,46,16023,11095,0,0,0,0,0,0,0,0,0,0},
      {3,47,5048,17595,0,0,0,0,0,0,0,0,0,0},
      {3,48,18995,4817,0,0,0,0,0,0,0,0,0,0},
      {3,49,16483,3536,0,0,0,0,0,0,0,0,0,0},
      {3,50,1439,16148,0,0,0,0,0,0,0,0,0,0},
      {3,51,3661,3039,0,0,0,0,0,0,0,0,0,0},
      {3,52,19010,18121,0,0,0,0,0,0,0,0,0,0},
      {3,53,8968,11793,0,0,0,0,0,0,0,0,0,0},
      {3,54,13427,18003,0,0,0,0,0,0,0,0,0,0},
      {3,55,5303,3083,0,0,0,0,0,0,0,0,0,0},
      {3,56,531,16668,0,0,0,0,0,0,0,0,0,0},
      {3,57,4771,6722,0,0,0,0,0,0,0,0,0,0},
      {3,58,5695,7960,0,0,0,0,0,0,0,0,0,0},
      {3,59,3589,14630,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_3_4N[135][13]=
    {
      {12,0,6385,7901,14611,13389,11200,3252,5243,2504,2722,821,7374},
      {12,1,11359,2698,357,13824,12772,7244,6752,15310,852,2001,11417},
      {12,2,7862,7977,6321,13612,12197,14449,15137,13860,1708,6399,13444},
      {12,3,1560,11804,6975,13292,3646,3812,8772,7306,5795,14327,7866},
      {12,4,7626,11407,14599,9689,1628,2113,10809,9283,1230,15241,4870},
      {12,5,1610,5699,15876,9446,12515,1400,6303,5411,14181,13925,7358},
      {12,6,4059,8836,3405,7853,7992,15336,5970,10368,10278,9675,4651},
      {12,7,4441,3963,9153,2109,12683,7459,12030,12221,629,15212,406},
      {12,8,6007,8411,5771,3497,543,14202,875,9186,6235,13908,3563},
      {12,9,3232,6625,4795,546,9781,2071,7312,3399,7250,4932,12652},
      {12,10,8820,10088,11090,7069,6585,13134,10158,7183,488,7455,9238},
      {12,11,1903,10818,119,215,7558,11046,10615,11545,14784,7961,15619},
      {12,12,3655,8736,4917,15874,5129,2134,15944,14768,7150,2692,1469},
      {12,13,8316,3820,505,8923,6757,806,7957,4216,15589,13244,2622},
      {12,14,14463,4852,15733,3041,11193,12860,13673,8152,6551,15108,8758},
      {3,15,3149,11981,0,0,0,0,0,0,0,0,0},
      {3,16,13416,6906,0,0,0,0,0,0,0,0,0},
      {3,17,13098,13352,0,0,0,0,0,0,0,0,0},
      {3,18,2009,14460,0,0,0,0,0,0,0,0,0},
      {3,19,7207,4314,0,0,0,0,0,0,0,0,0},
      {3,20,3312,3945,0,0,0,0,0,0,0,0,0},
      {3,21,4418,6248,0,0,0,0,0,0,0,0,0},
      {3,22,2669,13975,0,0,0,0,0,0,0,0,0},
      {3,23,7571,9023,0,0,0,0,0,0,0,0,0},
      {3,24,14172,2967,0,0,0,0,0,0,0,0,0},
      {3,25,7271,7138,0,0,0,0,0,0,0,0,0},
      {3,26,6135,13670,0,0,0,0,0,0,0,0,0},
      {3,27,7490,14559,0,0,0,0,0,0,0,0,0},
      {3,28,8657,2466,0,0,0,0,0,0,0,0,0},
      {3,29,8599,12834,0,0,0,0,0,0,0,0,0},
      {3,30,3470,3152,0,0,0,0,0,0,0,0,0},
      {3,31,13917,4365,0,0,0,0,0,0,0,0,0},
      {3,32,6024,13730,0,0,0,0,0,0,0,0,0},
      {3,33,10973,14182,0,0,0,0,0,0,0,0,0},
      {3,34,2464,13167,0,0,0,0,0,0,0,0,0},
      {3,35,5281,15049,0,0,0,0,0,0,0,0,0},
      {3,36,1103,1849,0,0,0,0,0,0,0,0,0},
      {3,37,2058,1069,0,0,0,0,0,0,0,0,0},
      {3,38,9654,6095,0,0,0,0,0,0,0,0,0},
      {3,39,14311,7667,0,0,0,0,0,0,0,0,0},
      {3,40,15617,8146,0,0,0,0,0,0,0,0,0},
      {3,41,4588,11218,0,0,0,0,0,0,0,0,0},
      {3,42,13660,6243,0,0,0,0,0,0,0,0,0},
      {3,43,8578,7874,0,0,0,0,0,0,0,0,0},
      {3,44,11741,2686,0,0,0,0,0,0,0,0,0},
      {3,0,1022,1264,0,0,0,0,0,0,0,0,0},
      {3,1,12604,9965,0,0,0,0,0,0,0,0,0},
      {3,2,8217,2707,0,0,0,0,0,0,0,0,0},
      {3,3,3156,11793,0,0,0,0,0,0,0,0,0},
      {3,4,354,1514,0,0,0,0,0,0,0,0,0},
      {3,5,6978,14058,0,0,0,0,0,0,0,0,0},
      {3,6,7922,16079,0,0,0,0,0,0,0,0,0},
      {3,7,15087,12138,0,0,0,0,0,0,0,0,0},
      {3,8,5053,6470,0,0,0,0,0,0,0,0,0},
      {3,9,12687,14932,0,0,0,0,0,0,0,0,0},
      {3,10,15458,1763,0,0,0,0,0,0,0,0,0},
      {3,11,8121,1721,0,0,0,0,0,0,0,0,0},
      {3,12,12431,549,0,0,0,0,0,0,0,0,0},
      {3,13,4129,7091,0,0,0,0,0,0,0,0,0},
      {3,14,1426,8415,0,0,0,0,0,0,0,0,0},
      {3,15,9783,7604,0,0,0,0,0,0,0,0,0},
      {3,16,6295,11329,0,0,0,0,0,0,0,0,0},
      {3,17,1409,12061,0,0,0,0,0,0,0,0,0},
      {3,18,8065,9087,0,0,0,0,0,0,0,0,0},
      {3,19,2918,8438,0,0,0,0,0,0,0,0,0},
      {3,20,1293,14115,0,0,0,0,0,0,0,0,0},
      {3,21,3922,13851,0,0,0,0,0,0,0,0,0},
      {3,22,3851,4000,0,0,0,0,0,0,0,0,0},
      {3,23,5865,1768,0,0,0,0,0,0,0,0,0},
      {3,24,2655,14957,0,0,0,0,0,0,0,0,0},
      {3,25,5565,6332,0,0,0,0,0,0,0,0,0},
      {3,26,4303,12631,0,0,0,0,0,0,0,0,0},
      {3,27,11653,12236,0,0,0,0,0,0,0,0,0},
      {3,28,16025,7632,0,0,0,0,0,0,0,0,0},
      {3,29,4655,14128,0,0,0,0,0,0,0,0,0},
      {3,30,9584,13123,0,0,0,0,0,0,0,0,0},
      {3,31,13987,9597,0,0,0,0,0,0,0,0,0},
      {3,32,15409,12110,0,0,0,0,0,0,0,0,0},
      {3,33,8754,15490,0,0,0,0,0,0,0,0,0},
      {3,34,7416,15325,0,0,0,0,0,0,0,0,0},
      {3,35,2909,15549,0,0,0,0,0,0,0,0,0},
      {3,36,2995,8257,0,0,0,0,0,0,0,0,0},
      {3,37,9406,4791,0,0,0,0,0,0,0,0,0},
      {3,38,11111,4854,0,0,0,0,0,0,0,0,0},
      {3,39,2812,8521,0,0,0,0,0,0,0,0,0},
      {3,40,8476,14717,0,0,0,0,0,0,0,0,0},
      {3,41,7820,15360,0,0,0,0,0,0,0,0,0},
      {3,42,1179,7939,0,0,0,0,0,0,0,0,0},
      {3,43,2357,8678,0,0,0,0,0,0,0,0,0},
      {3,44,7703,6216,0,0,0,0,0,0,0,0,0},
      {3,0,3477,7067,0,0,0,0,0,0,0,0,0},
      {3,1,3931,13845,0,0,0,0,0,0,0,0,0},
      {3,2,7675,12899,0,0,0,0,0,0,0,0,0},
      {3,3,1754,8187,0,0,0,0,0,0,0,0,0},
      {3,4,7785,1400,0,0,0,0,0,0,0,0,0},
      {3,5,9213,5891,0,0,0,0,0,0,0,0,0},
      {3,6,2494,7703,0,0,0,0,0,0,0,0,0},
      {3,7,2576,7902,0,0,0,0,0,0,0,0,0},
      {3,8,4821,15682,0,0,0,0,0,0,0,0,0},
      {3,9,10426,11935,0,0,0,0,0,0,0,0,0},
      {3,10,1810,904,0,0,0,0,0,0,0,0,0},
      {3,11,11332,9264,0,0,0,0,0,0,0,0,0},
      {3,12,11312,3570,0,0,0,0,0,0,0,0,0},
      {3,13,14916,2650,0,0,0,0,0,0,0,0,0},
      {3,14,7679,7842,0,0,0,0,0,0,0,0,0},
      {3,15,6089,13084,0,0,0,0,0,0,0,0,0},
      {3,16,3938,2751,0,0,0,0,0,0,0,0,0},
      {3,17,8509,4648,0,0,0,0,0,0,0,0,0},
      {3,18,12204,8917,0,0,0,0,0,0,0,0,0},
      {3,19,5749,12443,0,0,0,0,0,0,0,0,0},
      {3,20,12613,4431,0,0,0,0,0,0,0,0,0},
      {3,21,1344,4014,0,0,0,0,0,0,0,0,0},
      {3,22,8488,13850,0,0,0,0,0,0,0,0,0},
      {3,23,1730,14896,0,0,0,0,0,0,0,0,0},
      {3,24,14942,7126,0,0,0,0,0,0,0,0,0},
      {3,25,14983,8863,0,0,0,0,0,0,0,0,0},
      {3,26,6578,8564,0,0,0,0,0,0,0,0,0},
      {3,27,4947,396,0,0,0,0,0,0,0,0,0},
      {3,28,297,12805,0,0,0,0,0,0,0,0,0},
      {3,29,13878,6692,0,0,0,0,0,0,0,0,0},
      {3,30,11857,11186,0,0,0,0,0,0,0,0,0},
      {3,31,14395,11493,0,0,0,0,0,0,0,0,0},
      {3,32,16145,12251,0,0,0,0,0,0,0,0,0},
      {3,33,13462,7428,0,0,0,0,0,0,0,0,0},
      {3,34,14526,13119,0,0,0,0,0,0,0,0,0},
      {3,35,2535,11243,0,0,0,0,0,0,0,0,0},
      {3,36,6465,12690,0,0,0,0,0,0,0,0,0},
      {3,37,6872,9334,0,0,0,0,0,0,0,0,0},
      {3,38,15371,14023,0,0,0,0,0,0,0,0,0},
      {3,39,8101,10187,0,0,0,0,0,0,0,0,0},
      {3,40,11963,4848,0,0,0,0,0,0,0,0,0},
      {3,41,15125,6119,0,0,0,0,0,0,0,0,0},
      {3,42,8051,14465,0,0,0,0,0,0,0,0,0},
      {3,43,11139,5167,0,0,0,0,0,0,0,0,0},
      {3,44,2883,14521,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_4_5N[144][12]=
    {
      {11,0,149,11212,5575,6360,12559,8108,8505,408,10026,12828},
      {11,1,5237,490,10677,4998,3869,3734,3092,3509,7703,10305},
      {11,2,8742,5553,2820,7085,12116,10485,564,7795,2972,2157},
      {11,3,2699,4304,8350,712,2841,3250,4731,10105,517,7516},
      {11,4,12067,1351,11992,12191,11267,5161,537,6166,4246,2363},
      {11,5,6828,7107,2127,3724,5743,11040,10756,4073,1011,3422},
      {11,6,11259,1216,9526,1466,10816,940,3744,2815,11506,11573},
      {11,7,4549,11507,1118,1274,11751,5207,7854,12803,4047,6484},
      {11,8,8430,4115,9440,413,4455,2262,7915,12402,8579,7052},
      {11,9,3885,9126,5665,4505,2343,253,4707,3742,4166,1556},
      {11,10,1704,8936,6775,8639,8179,7954,8234,7850,8883,8713},
      {11,11,11716,4344,9087,11264,2274,8832,9147,11930,6054,5455},
      {11,12,7323,3970,10329,2170,8262,3854,2087,12899,9497,11700},
      {11,13,4418,1467,2490,5841,817,11453,533,11217,11962,5251},
      {11,14,1541,4525,7976,3457,9536,7725,3788,2982,6307,5997},
      {11,15,11484,2739,4023,12107,6516,551,2572,6628,8150,9852},
      {11,16,6070,1761,4627,6534,7913,3730,11866,1813,12306,8249},
      {11,17,12441,5489,8748,7837,7660,2102,11341,2936,6712,11977},
      {3,18,10155,4210,0,0,0,0,0,0,0,0},
      {3,19,1010,10483,0,0,0,0,0,0,0,0},
      {3,20,8900,10250,0,0,0,0,0,0,0,0},
      {3,21,10243,12278,0,0,0,0,0,0,0,0},
      {3,22,7070,4397,0,0,0,0,0,0,0,0},
      {3,23,12271,3887,0,0,0,0,0,0,0,0},
      {3,24,11980,6836,0,0,0,0,0,0,0,0},
      {3,25,9514,4356,0,0,0,0,0,0,0,0},
      {3,26,7137,10281,0,0,0,0,0,0,0,0},
      {3,27,11881,2526,0,0,0,0,0,0,0,0},
      {3,28,1969,11477,0,0,0,0,0,0,0,0},
      {3,29,3044,10921,0,0,0,0,0,0,0,0},
      {3,30,2236,8724,0,0,0,0,0,0,0,0},
      {3,31,9104,6340,0,0,0,0,0,0,0,0},
      {3,32,7342,8582,0,0,0,0,0,0,0,0},
      {3,33,11675,10405,0,0,0,0,0,0,0,0},
      {3,34,6467,12775,0,0,0,0,0,0,0,0},
      {3,35,3186,12198,0,0,0,0,0,0,0,0},
      {3,0,9621,11445,0,0,0,0,0,0,0,0},
      {3,1,7486,5611,0,0,0,0,0,0,0,0},
      {3,2,4319,4879,0,0,0,0,0,0,0,0},
      {3,3,2196,344,0,0,0,0,0,0,0,0},
      {3,4,7527,6650,0,0,0,0,0,0,0,0},
      {3,5,10693,2440,0,0,0,0,0,0,0,0},
      {3,6,6755,2706,0,0,0,0,0,0,0,0},
      {3,7,5144,5998,0,0,0,0,0,0,0,0},
      {3,8,11043,8033,0,0,0,0,0,0,0,0},
      {3,9,4846,4435,0,0,0,0,0,0,0,0},
      {3,10,4157,9228,0,0,0,0,0,0,0,0},
      {3,11,12270,6562,0,0,0,0,0,0,0,0},
      {3,12,11954,7592,0,0,0,0,0,0,0,0},
      {3,13,7420,2592,0,0,0,0,0,0,0,0},
      {3,14,8810,9636,0,0,0,0,0,0,0,0},
      {3,15,689,5430,0,0,0,0,0,0,0,0},
      {3,16,920,1304,0,0,0,0,0,0,0,0},
      {3,17,1253,11934,0,0,0,0,0,0,0,0},
      {3,18,9559,6016,0,0,0,0,0,0,0,0},
      {3,19,312,7589,0,0,0,0,0,0,0,0},
      {3,20,4439,4197,0,0,0,0,0,0,0,0},
      {3,21,4002,9555,0,0,0,0,0,0,0,0},
      {3,22,12232,7779,0,0,0,0,0,0,0,0},
      {3,23,1494,8782,0,0,0,0,0,0,0,0},
      {3,24,10749,3969,0,0,0,0,0,0,0,0},
      {3,25,4368,3479,0,0,0,0,0,0,0,0},
      {3,26,6316,5342,0,0,0,0,0,0,0,0},
      {3,27,2455,3493,0,0,0,0,0,0,0,0},
      {3,28,12157,7405,0,0,0,0,0,0,0,0},
      {3,29,6598,11495,0,0,0,0,0,0,0,0},
      {3,30,11805,4455,0,0,0,0,0,0,0,0},
      {3,31,9625,2090,0,0,0,0,0,0,0,0},
      {3,32,4731,2321,0,0,0,0,0,0,0,0},
      {3,33,3578,2608,0,0,0,0,0,0,0,0},
      {3,34,8504,1849,0,0,0,0,0,0,0,0},
      {3,35,4027,1151,0,0,0,0,0,0,0,0},
      {3,0,5647,4935,0,0,0,0,0,0,0,0},
      {3,1,4219,1870,0,0,0,0,0,0,0,0},
      {3,2,10968,8054,0,0,0,0,0,0,0,0},
      {3,3,6970,5447,0,0,0,0,0,0,0,0},
      {3,4,3217,5638,0,0,0,0,0,0,0,0},
      {3,5,8972,669,0,0,0,0,0,0,0,0},
      {3,6,5618,12472,0,0,0,0,0,0,0,0},
      {3,7,1457,1280,0,0,0,0,0,0,0,0},
      {3,8,8868,3883,0,0,0,0,0,0,0,0},
      {3,9,8866,1224,0,0,0,0,0,0,0,0},
      {3,10,8371,5972,0,0,0,0,0,0,0,0},
      {3,11,266,4405,0,0,0,0,0,0,0,0},
      {3,12,3706,3244,0,0,0,0,0,0,0,0},
      {3,13,6039,5844,0,0,0,0,0,0,0,0},
      {3,14,7200,3283,0,0,0,0,0,0,0,0},
      {3,15,1502,11282,0,0,0,0,0,0,0,0},
      {3,16,12318,2202,0,0,0,0,0,0,0,0},
      {3,17,4523,965,0,0,0,0,0,0,0,0},
      {3,18,9587,7011,0,0,0,0,0,0,0,0},
      {3,19,2552,2051,0,0,0,0,0,0,0,0},
      {3,20,12045,10306,0,0,0,0,0,0,0,0},
      {3,21,11070,5104,0,0,0,0,0,0,0,0},
      {3,22,6627,6906,0,0,0,0,0,0,0,0},
      {3,23,9889,2121,0,0,0,0,0,0,0,0},
      {3,24,829,9701,0,0,0,0,0,0,0,0},
      {3,25,2201,1819,0,0,0,0,0,0,0,0},
      {3,26,6689,12925,0,0,0,0,0,0,0,0},
      {3,27,2139,8757,0,0,0,0,0,0,0,0},
      {3,28,12004,5948,0,0,0,0,0,0,0,0},
      {3,29,8704,3191,0,0,0,0,0,0,0,0},
      {3,30,8171,10933,0,0,0,0,0,0,0,0},
      {3,31,6297,7116,0,0,0,0,0,0,0,0},
      {3,32,616,7146,0,0,0,0,0,0,0,0},
      {3,33,5142,9761,0,0,0,0,0,0,0,0},
      {3,34,10377,8138,0,0,0,0,0,0,0,0},
      {3,35,7616,5811,0,0,0,0,0,0,0,0},
      {3,0,7285,9863,0,0,0,0,0,0,0,0},
      {3,1,7764,10867,0,0,0,0,0,0,0,0},
      {3,2,12343,9019,0,0,0,0,0,0,0,0},
      {3,3,4414,8331,0,0,0,0,0,0,0,0},
      {3,4,3464,642,0,0,0,0,0,0,0,0},
      {3,5,6960,2039,0,0,0,0,0,0,0,0},
      {3,6,786,3021,0,0,0,0,0,0,0,0},
      {3,7,710,2086,0,0,0,0,0,0,0,0},
      {3,8,7423,5601,0,0,0,0,0,0,0,0},
      {3,9,8120,4885,0,0,0,0,0,0,0,0},
      {3,10,12385,11990,0,0,0,0,0,0,0,0},
      {3,11,9739,10034,0,0,0,0,0,0,0,0},
      {3,12,424,10162,0,0,0,0,0,0,0,0},
      {3,13,1347,7597,0,0,0,0,0,0,0,0},
      {3,14,1450,112,0,0,0,0,0,0,0,0},
      {3,15,7965,8478,0,0,0,0,0,0,0,0},
      {3,16,8945,7397,0,0,0,0,0,0,0,0},
      {3,17,6590,8316,0,0,0,0,0,0,0,0},
      {3,18,6838,9011,0,0,0,0,0,0,0,0},
      {3,19,6174,9410,0,0,0,0,0,0,0,0},
      {3,20,255,113,0,0,0,0,0,0,0,0},
      {3,21,6197,5835,0,0,0,0,0,0,0,0},
      {3,22,12902,3844,0,0,0,0,0,0,0,0},
      {3,23,4377,3505,0,0,0,0,0,0,0,0},
      {3,24,5478,8672,0,0,0,0,0,0,0,0},
      {3,25,4453,2132,0,0,0,0,0,0,0,0},
      {3,26,9724,1380,0,0,0,0,0,0,0,0},
      {3,27,12131,11526,0,0,0,0,0,0,0,0},
      {3,28,12323,9511,0,0,0,0,0,0,0,0},
      {3,29,8231,1752,0,0,0,0,0,0,0,0},
      {3,30,497,9022,0,0,0,0,0,0,0,0},
      {3,31,9288,3080,0,0,0,0,0,0,0,0},
      {3,32,2481,7515,0,0,0,0,0,0,0,0},
      {3,33,2696,268,0,0,0,0,0,0,0,0},
      {3,34,4023,12341,0,0,0,0,0,0,0,0},
      {3,35,7108,5553,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_5_6N[150][14]=
    {
      {13,0,4362,416,8909,4156,3216,3112,2560,2912,6405,8593,4969,6723},
      {13,1,2479,1786,8978,3011,4339,9313,6397,2957,7288,5484,6031,10217},
      {13,2,10175,9009,9889,3091,4985,7267,4092,8874,5671,2777,2189,8716},
      {13,3,9052,4795,3924,3370,10058,1128,9996,10165,9360,4297,434,5138},
      {13,4,2379,7834,4835,2327,9843,804,329,8353,7167,3070,1528,7311},
      {13,5,3435,7871,348,3693,1876,6585,10340,7144,5870,2084,4052,2780},
      {13,6,3917,3111,3476,1304,10331,5939,5199,1611,1991,699,8316,9960},
      {13,7,6883,3237,1717,10752,7891,9764,4745,3888,10009,4176,4614,1567},
      {13,8,10587,2195,1689,2968,5420,2580,2883,6496,111,6023,1024,4449},
      {13,9,3786,8593,2074,3321,5057,1450,3840,5444,6572,3094,9892,1512},
      {13,10,8548,1848,10372,4585,7313,6536,6379,1766,9462,2456,5606,9975},
      {13,11,8204,10593,7935,3636,3882,394,5968,8561,2395,7289,9267,9978},
      {13,12,7795,74,1633,9542,6867,7352,6417,7568,10623,725,2531,9115},
      {13,13,7151,2482,4260,5003,10105,7419,9203,6691,8798,2092,8263,3755},
      {13,14,3600,570,4527,200,9718,6771,1995,8902,5446,768,1103,6520},
      {3,15,6304,7621,0,0,0,0,0,0,0,0,0,0},
      {3,16,6498,9209,0,0,0,0,0,0,0,0,0,0},
      {3,17,7293,6786,0,0,0,0,0,0,0,0,0,0},
      {3,18,5950,1708,0,0,0,0,0,0,0,0,0,0},
      {3,19,8521,1793,0,0,0,0,0,0,0,0,0,0},
      {3,20,6174,7854,0,0,0,0,0,0,0,0,0,0},
      {3,21,9773,1190,0,0,0,0,0,0,0,0,0,0},
      {3,22,9517,10268,0,0,0,0,0,0,0,0,0,0},
      {3,23,2181,9349,0,0,0,0,0,0,0,0,0,0},
      {3,24,1949,5560,0,0,0,0,0,0,0,0,0,0},
      {3,25,1556,555,0,0,0,0,0,0,0,0,0,0},
      {3,26,8600,3827,0,0,0,0,0,0,0,0,0,0},
      {3,27,5072,1057,0,0,0,0,0,0,0,0,0,0},
      {3,28,7928,3542,0,0,0,0,0,0,0,0,0,0},
      {3,29,3226,3762,0,0,0,0,0,0,0,0,0,0},
      {3,0,7045,2420,0,0,0,0,0,0,0,0,0,0},
      {3,1,9645,2641,0,0,0,0,0,0,0,0,0},
      {3,2,2774,2452,0,0,0,0,0,0,0,0,0,0},
      {3,3,5331,2031,0,0,0,0,0,0,0,0,0,0},
      {3,4,9400,7503,0,0,0,0,0,0,0,0,0,0},
      {3,5,1850,2338,0,0,0,0,0,0,0,0,0,0},
      {3,6,10456,9774,0,0,0,0,0,0,0,0,0,0},
      {3,7,1692,9276,0,0,0,0,0,0,0,0,0,0},
      {3,8,10037,4038,0,0,0,0,0,0,0,0,0,0},
      {3,9,3964,338,0,0,0,0,0,0,0,0,0,0},
      {3,10,2640,5087,0,0,0,0,0,0,0,0,0,0},
      {3,11,858,3473,0,0,0,0,0,0,0,0,0,0},
      {3,12,5582,5683,0,0,0,0,0,0,0,0,0,0},
      {3,13,9523,916,0,0,0,0,0,0,0,0,0,0},
      {3,14,4107,1559,0,0,0,0,0,0,0,0,0,0},
      {3,15,4506,3491,0,0,0,0,0,0,0,0,0,0},
      {3,16,8191,4182,0,0,0,0,0,0,0,0,0,0},
      {3,17,10192,6157,0,0,0,0,0,0,0,0,0,0},
      {3,18,5668,3305,0,0,0,0,0,0,0,0,0,0},
      {3,19,3449,1540,0,0,0,0,0,0,0,0,0,0},
      {3,20,4766,2697,0,0,0,0,0,0,0,0,0,0},
      {3,21,4069,6675,0,0,0,0,0,0,0,0,0,0},
      {3,22,1117,1016,0,0,0,0,0,0,0,0,0,0},
      {3,23,5619,3085,0,0,0,0,0,0,0,0,0,0},
      {3,24,8483,8400,0,0,0,0,0,0,0,0,0,0},
      {3,25,8255,394,0,0,0,0,0,0,0,0,0,0},
      {3,26,6338,5042,0,0,0,0,0,0,0,0,0,0},
      {3,27,6174,5119,0,0,0,0,0,0,0,0,0,0},
      {3,28,7203,1989,0,0,0,0,0,0,0,0,0,0},
      {3,29,1781,5174,0,0,0,0,0,0,0,0,0,0},
      {3,0,1464,3559,0,0,0,0,0,0,0,0,0,0},
      {3,1,3376,4214,0,0,0,0,0,0,0,0,0,0},
      {3,2,7238,67,0,0,0,0,0,0,0,0,0,0},
      {3,3,10595,8831,0,0,0,0,0,0,0,0,0,0},
      {3,4,1221,6513,0,0,0,0,0,0,0,0,0,0},
      {3,5,5300,4652,0,0,0,0,0,0,0,0,0,0},
      {3,6,1429,9749,0,0,0,0,0,0,0,0,0,0},
      {3,7,7878,5131,0,0,0,0,0,0,0,0,0,0},
      {3,8,4435,10284,0,0,0,0,0,0,0,0,0,0},
      {3,9,6331,5507,0,0,0,0,0,0,0,0,0,0},
      {3,10,6662,4941,0,0,0,0,0,0,0,0,0,0},
      {3,11,9614,10238,0,0,0,0,0,0,0,0,0,0},
      {3,12,8400,8025,0,0,0,0,0,0,0,0,0,0},
      {3,13,9156,5630,0,0,0,0,0,0,0,0,0,0},
      {3,14,7067,8878,0,0,0,0,0,0,0,0,0,0},
      {3,15,9027,3415,0,0,0,0,0,0,0,0,0,0},
      {3,16,1690,3866,0,0,0,0,0,0,0,0,0,0},
      {3,17,2854,8469,0,0,0,0,0,0,0,0,0,0},
      {3,18,6206,630,0,0,0,0,0,0,0,0,0,0},
      {3,19,363,5453,0,0,0,0,0,0,0,0,0,0},
      {3,20,4125,7008,0,0,0,0,0,0,0,0,0,0},
      {3,21,1612,6702,0,0,0,0,0,0,0,0,0,0},
      {3,22,9069,9226,0,0,0,0,0,0,0,0,0,0},
      {3,23,5767,4060,0,0,0,0,0,0,0,0,0,0},
      {3,24,3743,9237,0,0,0,0,0,0,0,0,0,0},
      {3,25,7018,5572,0,0,0,0,0,0,0,0,0,0},
      {3,26,8892,4536,0,0,0,0,0,0,0,0,0,0},
      {3,27,853,6064,0,0,0,0,0,0,0,0,0,0},
      {3,28,8069,5893,0,0,0,0,0,0,0,0,0,0},
      {3,29,2051,2885,0,0,0,0,0,0,0,0,0,0},
      {3,0,10691,3153,0,0,0,0,0,0,0,0,0,0},
      {3,1,3602,4055,0,0,0,0,0,0,0,0,0,0},
      {3,2,328,1717,0,0,0,0,0,0,0,0,0,0},
      {3,3,2219,9299,0,0,0,0,0,0,0,0,0,0},
      {3,4,1939,7898,0,0,0,0,0,0,0,0,0,0},
      {3,5,617,206,0,0,0,0,0,0,0,0,0,0},
      {3,6,8544,1374,0,0,0,0,0,0,0,0,0,0},
      {3,7,10676,3240,0,0,0,0,0,0,0,0,0,0},
      {3,8,6672,9489,0,0,0,0,0,0,0,0,0,0},
      {3,9,3170,7457,0,0,0,0,0,0,0,0,0,0},
      {3,10,7868,5731,0,0,0,0,0,0,0,0,0,0},
      {3,11,6121,10732,0,0,0,0,0,0,0,0,0,0},
      {3,12,4843,9132,0,0,0,0,0,0,0,0,0,0},
      {3,13,580,9591,0,0,0,0,0,0,0,0,0,0},
      {3,14,6267,9290,0,0,0,0,0,0,0,0,0,0},
      {3,15,3009,2268,0,0,0,0,0,0,0,0,0,0},
      {3,16,195,2419,0,0,0,0,0,0,0,0,0,0},
      {3,17,8016,1557,0,0,0,0,0,0,0,0,0,0},
      {3,18,1516,9195,0,0,0,0,0,0,0,0,0,0},
      {3,19,8062,9064,0,0,0,0,0,0,0,0,0,0},
      {3,20,2095,8968,0,0,0,0,0,0,0,0,0,0},
      {3,21,753,7326,0,0,0,0,0,0,0,0,0,0},
      {3,22,6291,3833,0,0,0,0,0,0,0,0,0,0},
      {3,23,2614,7844,0,0,0,0,0,0,0,0,0,0},
      {3,24,2303,646,0,0,0,0,0,0,0,0,0,0},
      {3,25,2075,611,0,0,0,0,0,0,0,0,0,0},
      {3,26,4687,362,0,0,0,0,0,0,0,0,0,0},
      {3,27,8684,9940,0,0,0,0,0,0,0,0,0,0},
      {3,28,4830,2065,0,0,0,0,0,0,0,0,0,0},
      {3,29,7038,1363,0,0,0,0,0,0,0,0,0,0},
      {3,0,1769,7837,0,0,0,0,0,0,0,0,0,0},
      {3,1,3801,1689,0,0,0,0,0,0,0,0,0,0},
      {3,2,10070,2359,0,0,0,0,0,0,0,0,0,0},
      {3,3,3667,9918,0,0,0,0,0,0,0,0,0,0},
      {3,4,1914,6920,0,0,0,0,0,0,0,0,0,0},
      {3,5,4244,5669,0,0,0,0,0,0,0,0,0,0},
      {3,6,10245,7821,0,0,0,0,0,0,0,0,0,0},
      {3,7,7648,3944,0,0,0,0,0,0,0,0,0,0},
      {3,8,3310,5488,0,0,0,0,0,0,0,0,0,0},
      {3,9,6346,9666,0,0,0,0,0,0,0,0,0,0},
      {3,10,7088,6122,0,0,0,0,0,0,0,0,0,0},
      {3,11,1291,7827,0,0,0,0,0,0,0,0,0,0},
      {3,12,10592,8945,0,0,0,0,0,0,0,0,0,0},
      {3,13,3609,7120,0,0,0,0,0,0,0,0,0,0},
      {3,14,9168,9112,0,0,0,0,0,0,0,0,0,0},
      {3,15,6203,8052,0,0,0,0,0,0,0,0,0,0},
      {3,16,3330,2895,0,0,0,0,0,0,0,0,0,0},
      {3,17,4264,10563,0,0,0,0,0,0,0,0,0,0},
      {3,18,10556,6496,0,0,0,0,0,0,0,0,0,0},
      {3,19,8807,7645,0,0,0,0,0,0,0,0,0,0},
      {3,20,1999,4530,0,0,0,0,0,0,0,0,0,0},
      {3,21,9202,6818,0,0,0,0,0,0,0,0,0,0},
      {3,22,3403,1734,0,0,0,0,0,0,0,0,0,0},
      {3,23,2106,9023,0,0,0,0,0,0,0,0,0,0},
      {3,24,6881,3883,0,0,0,0,0,0,0,0,0,0},
      {3,25,3895,2171,0,0,0,0,0,0,0,0,0,0},
      {3,26,4062,6424,0,0,0,0,0,0,0,0,0,0},
      {3,27,3755,9536,0,0,0,0,0,0,0,0,0,0},
      {3,28,4683,2131,0,0,0,0,0,0,0,0,0,0},
      {3,29,7347,8027,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_8_9N[160][5]=
    {
      {4,0,6235,2848,3222},
      {4,1,5800,3492,5348},
      {4,2,2757,927,90},
      {4,3,6961,4516,4739},
      {4,4,1172,3237,6264},
      {4,5,1927,2425,3683},
      {4,6,3714,6309,2495},
      {4,7,3070,6342,7154},
      {4,8,2428,613,3761},
      {4,9,2906,264,5927},
      {4,10,1716,1950,4273},
      {4,11,4613,6179,3491},
      {4,12,4865,3286,6005},
      {4,13,1343,5923,3529},
      {4,14,4589,4035,2132},
      {4,15,1579,3920,6737},
      {4,16,1644,1191,5998},
      {4,17,1482,2381,4620},
      {4,18,6791,6014,6596},
      {4,19,2738,5918,3786},
      {3,0,5156,6166,0},
      {3,1,1504,4356,0},
      {3,2,130,1904,0},
      {3,3,6027,3187,0},
      {3,4,6718,759,0},
      {3,5,6240,2870,0},
      {3,6,2343,1311,0},
      {3,7,1039,5465,0},
      {3,8,6617,2513,0},
      {3,9,1588,5222,0},
      {3,10,6561,535,0},
      {3,11,4765,2054,0},
      {3,12,5966,6892,0},
      {3,13,1969,3869,0},
      {3,14,3571,2420,0},
      {3,15,4632,981,0},
      {3,16,3215,4163,0},
      {3,17,973,3117,0},
      {3,18,3802,6198,0},
      {3,19,3794,3948,0},
      {3,0,3196,6126,0},
      {3,1,573,1909,0},
      {3,2,850,4034,0},
      {3,3,5622,1601,0},
      {3,4,6005,524,0},
      {3,5,5251,5783,0},
      {3,6,172,2032,0},
      {3,7,1875,2475,0},
      {3,8,497,1291,0},
      {3,9,2566,3430,0},
      {3,10,1249,740,0},
      {3,11,2944,1948,0},
      {3,12,6528,2899,0},
      {3,13,2243,3616,0},
      {3,14,867,3733,0},
      {3,15,1374,4702,0},
      {3,16,4698,2285,0},
      {3,17,4760,3917,0},
      {3,18,1859,4058,0},
      {3,19,6141,3527,0},
      {3,0,2148,5066,0},
      {3,1,1306,145,0},
      {3,2,2319,871,0},
      {3,3,3463,1061,0},
      {3,4,5554,6647,0},
      {3,5,5837,339,0},
      {3,6,5821,4932,0},
      {3,7,6356,4756,0},
      {3,8,3930,418,0},
      {3,9,211,3094,0},
      {3,10,1007,4928,0},
      {3,11,3584,1235,0},
      {3,12,6982,2869,0},
      {3,13,1612,1013,0},
      {3,14,953,4964,0},
      {3,15,4555,4410,0},
      {3,16,4925,4842,0},
      {3,17,5778,600,0},
      {3,18,6509,2417,0},
      {3,19,1260,4903,0},
      {3,0,3369,3031,0},
      {3,1,3557,3224,0},
      {3,2,3028,583,0},
      {3,3,3258,440,0},
      {3,4,6226,6655,0},
      {3,5,4895,1094,0},
      {3,6,1481,6847,0},
      {3,7,4433,1932,0},
      {3,8,2107,1649,0},
      {3,9,2119,2065,0},
      {3,10,4003,6388,0},
      {3,11,6720,3622,0},
      {3,12,3694,4521,0},
      {3,13,1164,7050,0},
      {3,14,1965,3613,0},
      {3,15,4331,66,0},
      {3,16,2970,1796,0},
      {3,17,4652,3218,0},
      {3,18,1762,4777,0},
      {3,19,5736,1399,0},
      {3,0,970,2572,0},
      {3,1,2062,6599,0},
      {3,2,4597,4870,0},
      {3,3,1228,6913,0},
      {3,4,4159,1037,0},
      {3,5,2916,2362,0},
      {3,6,395,1226,0},
      {3,7,6911,4548,0},
      {3,8,4618,2241,0},
      {3,9,4120,4280,0},
      {3,10,5825,474,0},
      {3,11,2154,5558,0},
      {3,12,3793,5471,0},
      {3,13,5707,1595,0},
      {3,14,1403,325,0},
      {3,15,6601,5183,0},
      {3,16,6369,4569,0},
      {3,17,4846,896,0},
      {3,18,7092,6184,0},
      {3,19,6764,7127,0},
      {3,0,6358,1951,0},
      {3,1,3117,6960,0},
      {3,2,2710,7062,0},
      {3,3,1133,3604,0},
      {3,4,3694,657,0},
      {3,5,1355,110,0},
      {3,6,3329,6736,0},
      {3,7,2505,3407,0},
      {3,8,2462,4806,0},
      {3,9,4216,214,0},
      {3,10,5348,5619,0},
      {3,11,6627,6243,0},
      {3,12,2644,5073,0},
      {3,13,4212,5088,0},
      {3,14,3463,3889,0},
      {3,15,5306,478,0},
      {3,16,4320,6121,0},
      {3,17,3961,1125,0},
      {3,18,5699,1195,0},
      {3,19,6511,792,0},
      {3,0,3934,2778,0},
      {3,1,3238,6587,0},
      {3,2,1111,6596,0},
      {3,3,1457,6226,0},
      {3,4,1446,3885,0},
      {3,5,3907,4043,0},
      {3,6,6839,2873,0},
      {3,7,1733,5615,0},
      {3,8,5202,4269,0},
      {3,9,3024,4722,0},
      {3,10,5445,6372,0},
      {3,11,370,1828,0},
      {3,12,4695,1600,0},
      {3,13,680,2074,0},
      {3,14,1801,6690,0},
      {3,15,2669,1377,0},
      {3,16,2463,1681,0},
      {3,17,5972,5171,0},
      {3,18,5728,4284,0},
      {3,19,1696,1459,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_9_10N[162][5]=
    {
      {4,0,5611,2563,2900},
      {4,1,5220,3143,4813},
      {4,2,2481,834,81},
      {4,3,6265,4064,4265},
      {4,4,1055,2914,5638},
      {4,5,1734,2182,3315},
      {4,6,3342,5678,2246},
      {4,7,2185,552,3385},
      {4,8,2615,236,5334},
      {4,9,1546,1755,3846},
      {4,10,4154,5561,3142},
      {4,11,4382,2957,5400},
      {4,12,1209,5329,3179},
      {4,13,1421,3528,6063},
      {4,14,1480,1072,5398},
      {4,15,3843,1777,4369},
      {4,16,1334,2145,4163},
      {4,17,2368,5055,260},
      {3,0,6118,5405,0},
      {3,1,2994,4370,0},
      {3,2,3405,1669,0},
      {3,3,4640,5550,0},
      {3,4,1354,3921,0},
      {3,5,117,1713,0},
      {3,6,5425,2866,0},
      {3,7,6047,683,0},
      {3,8,5616,2582,0},
      {3,9,2108,1179,0},
      {3,10,933,4921,0},
      {3,11,5953,2261,0},
      {3,12,1430,4699,0},
      {3,13,5905,480,0},
      {3,14,4289,1846,0},
      {3,15,5374,6208,0},
      {3,16,1775,3476,0},
      {3,17,3216,2178,0},
      {3,0,4165,884,0},
      {3,1,2896,3744,0},
      {3,2,874,2801,0},
      {3,3,3423,5579,0},
      {3,4,3404,3552,0},
      {3,5,2876,5515,0},
      {3,6,516,1719,0},
      {3,7,765,3631,0},
      {3,8,5059,1441,0},
      {3,9,5629,598,0},
      {3,10,5405,473,0},
      {3,11,4724,5210,0},
      {3,12,155,1832,0},
      {3,13,1689,2229,0},
      {3,14,449,1164,0},
      {3,15,2308,3088,0},
      {3,16,1122,669,0},
      {3,17,2268,5758,0},
      {3,0,5878,2609,0},
      {3,1,782,3359,0},
      {3,2,1231,4231,0},
      {3,3,4225,2052,0},
      {3,4,4286,3517,0},
      {3,5,5531,3184,0},
      {3,6,1935,4560,0},
      {3,7,1174,131,0},
      {3,8,3115,956,0},
      {3,9,3129,1088,0},
      {3,10,5238,4440,0},
      {3,11,5722,4280,0},
      {3,12,3540,375,0},
      {3,13,191,2782,0},
      {3,14,906,4432,0},
      {3,15,3225,1111,0},
      {3,16,6296,2583,0},
      {3,17,1457,903,0},
      {3,0,855,4475,0},
      {3,1,4097,3970,0},
      {3,2,4433,4361,0},
      {3,3,5198,541,0},
      {3,4,1146,4426,0},
      {3,5,3202,2902,0},
      {3,6,2724,525,0},
      {3,7,1083,4124,0},
      {3,8,2326,6003,0},
      {3,9,5605,5990,0},
      {3,10,4376,1579,0},
      {3,11,4407,984,0},
      {3,12,1332,6163,0},
      {3,13,5359,3975,0},
      {3,14,1907,1854,0},
      {3,15,3601,5748,0},
      {3,16,6056,3266,0},
      {3,17,3322,4085,0},
      {3,0,1768,3244,0},
      {3,1,2149,144,0},
      {3,2,1589,4291,0},
      {3,3,5154,1252,0},
      {3,4,1855,5939,0},
      {3,5,4820,2706,0},
      {3,6,1475,3360,0},
      {3,7,4266,693,0},
      {3,8,4156,2018,0},
      {3,9,2103,752,0},
      {3,10,3710,3853,0},
      {3,11,5123,931,0},
      {3,12,6146,3323,0},
      {3,13,1939,5002,0},
      {3,14,5140,1437,0},
      {3,15,1263,293,0},
      {3,16,5949,4665,0},
      {3,17,4548,6380,0},
      {3,0,3171,4690,0},
      {3,1,5204,2114,0},
      {3,2,6384,5565,0},
      {3,3,5722,1757,0},
      {3,4,2805,6264,0},
      {3,5,1202,2616,0},
      {3,6,1018,3244,0},
      {3,7,4018,5289,0},
      {3,8,2257,3067,0},
      {3,9,2483,3073,0},
      {3,10,1196,5329,0},
      {3,11,649,3918,0},
      {3,12,3791,4581,0},
      {3,13,5028,3803,0},
      {3,14,3119,3506,0},
      {3,15,4779,431,0},
      {3,16,3888,5510,0},
      {3,17,4387,4084,0},
      {3,0,5836,1692,0},
      {3,1,5126,1078,0},
      {3,2,5721,6165,0},
      {3,3,3540,2499,0},
      {3,4,2225,6348,0},
      {3,5,1044,1484,0},
      {3,6,6323,4042,0},
      {3,7,1313,5603,0},
      {3,8,1303,3496,0},
      {3,9,3516,3639,0},
      {3,10,5161,2293,0},
      {3,11,4682,3845,0},
      {3,12,3045,643,0},
      {3,13,2818,2616,0},
      {3,14,3267,649,0},
      {3,15,6236,593,0},
      {3,16,646,2948,0},
      {3,17,4213,1442,0},
      {3,0,5779,1596,0},
      {3,1,2403,1237,0},
      {3,2,2217,1514,0},
      {3,3,5609,716,0},
      {3,4,5155,3858,0},
      {3,5,1517,1312,0},
      {3,6,2554,3158,0},
      {3,7,5280,2643,0},
      {3,8,4990,1353,0},
      {3,9,5648,1170,0},
      {3,10,1152,4366,0},
      {3,11,3561,5368,0},
      {3,12,3581,1411,0},
      {3,13,5647,4661,0},
      {3,14,1542,5401,0},
      {3,15,5078,2687,0},
      {3,16,316,1755,0},
      {3,17,3392,1991,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_2_9N[40][12]=
    {
      {11,5332,8018,35444,13098,9655,41945,44273,22741,9371,8727,43219},
      {11,41410,43593,14611,46707,16041,1459,29246,12748,32996,676,46909},
      {11,9340,35072,35640,17537,10512,44339,30965,25175,9918,21079,29835},
      {11,3332,12088,47966,25168,50180,42842,40914,46726,17073,41812,34356},
      {11,15159,2209,7971,22590,20020,27567,4853,10294,38839,15314,49808},
      {11,20936,14497,23365,22630,38728,28361,34659,956,8559,44957,22222},
      {11,28043,4641,25208,47039,30612,25796,14661,44139,27335,12884,6980},
      {11,32584,33453,1867,20185,36106,30357,809,28513,46045,27862,4802},
      {11,43744,13375,36066,23604,30766,6233,45051,23660,20815,19525,25207},
      {11,27522,3854,9311,21925,41107,25773,26323,24237,24344,46187,44503},
      {11,10256,20038,12177,26635,5214,14191,34404,45807,4938,4173,31344},
      {11,32043,26501,46725,4648,16718,31060,26633,19036,14222,13886,26535},
      {11,18103,8498,36814,34600,36495,36712,29833,27396,11877,42861,1834},
      {11,36592,1645,3649,30521,14674,3630,890,13307,41412,24682,9907},
      {11,4401,44543,13784,5828,32862,25179,29736,39614,5186,49749,38317},
      {11,41460,39101,50080,40137,32691,26528,35332,44067,8467,14286,10470},
      {11,12211,34019,37870,36918,36419,33153,50070,41498,47741,30538,12342},
      {11,33751,23988,33624,41882,34075,25552,3106,17611,13190,29336,312},
      {11,5667,35483,35460,16153,37267,28308,50009,46345,34204,32756,38243},
      {11,5657,24157,36834,6890,49576,46244,43875,16738,47225,2944,36882},
      {3,30341,48485,3700,0,0,0,0,0,0,0,0},
      {3,14451,20438,18875,0,0,0,0,0,0,0,0},
      {3,13634,41138,42962,0,0,0,0,0,0,0,0},
      {3,46459,13369,27974,0,0,0,0,0,0,0,0},
      {3,21493,14629,2369,0,0,0,0,0,0,0,0},
      {3,11351,40226,42457,0,0,0,0,0,0,0,0},
      {3,34749,39000,3912,0,0,0,0,0,0,0,0},
      {3,18128,46776,47055,0,0,0,0,0,0,0,0},
      {3,2221,26806,11345,0,0,0,0,0,0,0,0},
      {3,35143,630,2229,0,0,0,0,0,0,0,0},
      {3,44009,41295,34646,0,0,0,0,0,0,0,0},
      {3,32163,16657,26544,0,0,0,0,0,0,0,0},
      {3,31770,23641,43623,0,0,0,0,0,0,0,0},
      {3,45826,10902,39490,0,0,0,0,0,0,0,0},
      {3,7514,20480,28511,0,0,0,0,0,0,0,0},
      {3,11429,19834,35430,0,0,0,0,0,0,0,0},
      {3,50112,38163,5738,0,0,0,0,0,0,0,0},
      {3,16191,16862,6783,0,0,0,0,0,0,0,0},
      {3,6085,39149,34988,0,0,0,0,0,0,0,0},
      {3,41497,32023,28688,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_13_45N[52][13]=
    {
      {12,15210,4519,18217,34427,18474,16813,28246,17687,44527,31465,13004,43601},
      {12,28576,13611,24294,15041,503,11393,26290,9278,19484,20742,13226,28322},
      {12,32651,27323,22368,15522,37576,20607,20152,19741,26700,31696,21061,35991},
      {12,44168,27910,31104,34776,38835,45450,40002,31522,7807,26330,2410,44983},
      {12,15861,39215,14631,42584,26502,41864,27885,32276,29049,16878,37480,42550},
      {12,38795,13012,7912,4058,23869,3325,42889,19921,13826,40323,18162,10005},
      {12,35100,5483,7629,35166,1239,10772,5289,286,16172,41843,42612,38493},
      {12,11997,40340,19047,16236,43557,9104,24032,2915,19265,36209,6443,40947},
      {12,43527,29675,4195,31926,35392,20400,7515,45806,36068,33079,37325,6301},
      {12,4580,20492,40934,14478,8238,2425,28901,43602,7224,17640,28259,6850},
      {12,41859,14006,19132,5690,16223,11575,30562,44797,3759,9833,36529,21084},
      {12,45546,16044,26763,13559,29092,41595,5726,13733,9164,15354,20145,10655},
      {12,24076,40883,13424,30325,40589,32367,36270,9286,40151,8501,3871,22109},
      {12,26239,29805,5358,44835,11609,3899,9760,39600,43422,13295,45431,14515},
      {12,5392,37010,12386,40193,21492,45146,12376,41952,43153,45733,718,35726},
      {12,33884,38006,16927,20958,25413,44561,11245,12984,35198,30977,31916,10657},
      {12,1412,1048,14965,31879,29967,41000,32087,22,34773,768,27289,19898},
      {12,43051,6964,31807,4119,33509,15950,6304,2813,35192,38282,39710,26356},
      {12,9889,18957,6355,18770,40381,1876,38889,17958,20309,10744,1744,228},
      {12,41543,36505,32795,12454,8520,4916,22313,1363,13010,8770,17057,8694},
      {12,22987,29564,13804,3110,1382,33844,15117,42314,36045,25295,28421,22044},
      {12,15951,42952,17458,6926,21257,41243,8662,17046,15054,15302,16964,40079},
      {12,13359,45754,16715,9586,10960,25406,14675,8880,5087,12303,28993,13571},
      {12,24824,31012,4121,808,30962,28736,11013,20488,7715,7637,6217,25114},
      {3,23615,5760,5554,0,0,0,0,0,0,0,0,0},
      {3,18072,21605,39242,0,0,0,0,0,0,0,0,0},
      {3,24190,6592,12281,0,0,0,0,0,0,0,0,0},
      {3,44681,6563,7001,0,0,0,0,0,0,0,0,0},
      {3,18291,19605,33476,0,0,0,0,0,0,0,0,0},
      {3,2884,30927,18430,0,0,0,0,0,0,0,0,0},
      {3,23674,36414,30649,0,0,0,0,0,0,0,0,0},
      {3,15364,22089,19757,0,0,0,0,0,0,0,0,0},
      {3,41162,14454,17627,0,0,0,0,0,0,0,0,0},
      {3,16676,28573,22163,0,0,0,0,0,0,0,0,0},
      {3,8851,36803,27589,0,0,0,0,0,0,0,0,0},
      {3,40049,476,1413,0,0,0,0,0,0,0,0,0},
      {3,41013,34505,33296,0,0,0,0,0,0,0,0,0},
      {3,29782,38018,42124,0,0,0,0,0,0,0,0,0},
      {3,22625,7485,11772,0,0,0,0,0,0,0,0,0},
      {3,2052,37567,14082,0,0,0,0,0,0,0,0,0},
      {3,30106,43203,20858,0,0,0,0,0,0,0,0,0},
      {3,7399,3796,22396,0,0,0,0,0,0,0,0,0},
      {3,38745,792,44483,0,0,0,0,0,0,0,0,0},
      {3,28268,33355,41030,0,0,0,0,0,0,0,0,0},
      {3,30098,37269,12871,0,0,0,0,0,0,0,0,0},
      {3,35769,33119,16738,0,0,0,0,0,0,0,0,0},
      {3,3307,43434,13244,0,0,0,0,0,0,0,0,0},
      {3,17852,9133,23190,0,0,0,0,0,0,0,0,0},
      {3,35184,20115,24202,0,0,0,0,0,0,0,0,0},
      {3,14760,43026,19425,0,0,0,0,0,0,0,0,0},
      {3,26414,16821,6625,0,0,0,0,0,0,0,0,0},
      {3,30362,35769,42608,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_9_20N[81][13]=
    {
      {12,30649,35117,23181,15492,2367,31230,9368,13541,6608,23384,18300,5905},
      {12,1961,8950,20589,17688,9641,1877,4937,15293,24864,14876,6516,10165},
      {12,4229,26034,28862,8265,27847,3,22728,13946,27162,26003,17696,13261},
      {12,31719,25669,17149,17377,33106,12630,4814,16334,1480,32952,11187,3849},
      {12,30186,20938,7946,23283,11042,28080,26642,34560,11302,4991,5121,6879},
      {12,13445,22794,18048,15116,5657,9853,15581,34960,13240,11176,17937,25081},
      {12,4868,28235,30286,29706,7073,6773,10390,27002,13015,7388,14772,19581},
      {12,11765,16642,11431,19588,20154,8027,29758,5501,6398,4268,21337,21136},
      {12,2275,7899,25943,12939,14478,20369,22877,3591,12217,19130,24252,32444},
      {12,24599,21382,4689,3524,11304,20423,13677,19639,10577,28279,22330,30722},
      {12,21622,26233,3921,17722,6843,5999,8186,2355,33632,34632,30285,9616},
      {12,19909,30417,19587,27853,13896,3689,155,20457,33362,21739,22779,33862},
      {12,3713,32975,9403,2836,23109,11099,3505,14562,17309,26470,4843,12279},
      {12,24216,26340,22073,32570,12936,19797,21801,8918,7999,24408,5783,25190},
      {12,8817,29367,17017,6208,21402,2280,2110,7975,32039,34605,1235,912},
      {12,23116,33017,31405,638,4707,31760,18043,3507,11989,26632,32829,11262},
      {12,9274,2553,10697,13507,15323,27080,3752,33191,12363,24664,14068,1416},
      {12,21670,26696,18570,25197,1517,7765,32686,6572,30901,28242,17802,24056},
      {12,35388,26895,8023,31249,29290,13440,7156,17367,21472,27219,14447,9655},
      {12,11100,27918,2900,33262,15301,4664,15728,1185,24818,32995,31108,16368},
      {12,34978,31690,30464,13044,5492,10047,2768,14336,30880,32780,10993,24750},
      {12,7022,19718,26036,19145,21177,33949,17135,5193,33718,2539,13920,25537},
      {12,918,18514,14530,13699,11902,22721,8335,35346,24655,3332,14708,20822},
      {12,11191,24064,32825,12321,11771,23299,31325,25526,16785,22212,34075,9066},
      {12,31209,27819,5974,19918,26831,33338,26647,9480,28489,7827,18562,2401},
      {12,17395,23192,10277,28458,23028,18793,10463,10740,616,24647,4153,10128},
      {12,2873,22381,8132,18239,31614,4193,32313,7575,25801,27591,19872,17992},
      {4,4609,9114,14764,13516,0,0,0,0,0,0,0,0},
      {4,19192,9882,13112,16075,0,0,0,0,0,0,0,0},
      {4,12510,28902,8784,32679,0,0,0,0,0,0,0,0},
      {4,4578,34533,30609,25543,0,0,0,0,0,0,0,0},
      {4,13739,3465,5330,999,0,0,0,0,0,0,0,0},
      {4,33254,13085,5001,29061,0,0,0,0,0,0,0,0},
      {4,28369,79,17750,13399,0,0,0,0,0,0,0,0},
      {4,24851,9524,30966,10422,0,0,0,0,0,0,0,0},
      {4,18251,34810,12259,25103,0,0,0,0,0,0,0,0},
      {3,25193,16945,1059,0,0,0,0,0,0,0,0,0},
      {3,11266,13612,30508,0,0,0,0,0,0,0,0,0},
      {3,24778,25364,1322,0,0,0,0,0,0,0,0,0},
      {3,14492,11111,13693,0,0,0,0,0,0,0,0,0},
      {3,15125,8205,1749,0,0,0,0,0,0,0,0,0},
      {3,8494,9902,9395,0,0,0,0,0,0,0,0,0},
      {3,23936,3981,22799,0,0,0,0,0,0,0,0,0},
      {3,28448,28076,26544,0,0,0,0,0,0,0,0,0},
      {3,19652,13424,8915,0,0,0,0,0,0,0,0,0},
      {3,2885,11356,3241,0,0,0,0,0,0,0,0,0},
      {3,1609,10284,24350,0,0,0,0,0,0,0,0,0},
      {3,2462,19358,15717,0,0,0,0,0,0,0,0,0},
      {3,29327,15960,14743,0,0,0,0,0,0,0,0,0},
      {3,5388,32927,1288,0,0,0,0,0,0,0,0,0},
      {3,19074,6322,32214,0,0,0,0,0,0,0,0,0},
      {3,34208,30535,35462,0,0,0,0,0,0,0,0,0},
      {3,23415,20836,21819,0,0,0,0,0,0,0,0,0},
      {3,17986,12196,30030,0,0,0,0,0,0,0,0,0},
      {3,8422,2647,5710,0,0,0,0,0,0,0,0,0},
      {3,3200,23132,23337,0,0,0,0,0,0,0,0,0},
      {3,22307,29841,4813,0,0,0,0,0,0,0,0,0},
      {3,15309,26942,29970,0,0,0,0,0,0,0,0,0},
      {3,23288,7493,3005,0,0,0,0,0,0,0,0,0},
      {3,20661,34283,33192,0,0,0,0,0,0,0,0,0},
      {3,23033,9541,6424,0,0,0,0,0,0,0,0,0},
      {3,22003,24665,5534,0,0,0,0,0,0,0,0,0},
      {3,4684,1411,33340,0,0,0,0,0,0,0,0,0},
      {3,26042,6426,3808,0,0,0,0,0,0,0,0,0},
      {3,285,21942,14302,0,0,0,0,0,0,0,0,0},
      {3,16023,6825,20084,0,0,0,0,0,0,0,0,0},
      {3,34878,12295,32028,0,0,0,0,0,0,0,0,0},
      {3,2591,178,24107,0,0,0,0,0,0,0,0,0},
      {3,16379,2912,9912,0,0,0,0,0,0,0,0,0},
      {3,15375,16120,28375,0,0,0,0,0,0,0,0,0},
      {3,20170,726,11291,0,0,0,0,0,0,0,0,0},
      {3,8185,13471,8448,0,0,0,0,0,0,0,0,0},
      {3,23205,14239,17896,0,0,0,0,0,0,0,0,0},
      {3,17950,19308,1591,0,0,0,0,0,0,0,0,0},
      {3,3170,23836,18879,0,0,0,0,0,0,0,0,0},
      {3,12853,10678,18431,0,0,0,0,0,0,0,0,0},
      {3,21157,31624,3153,0,0,0,0,0,0,0,0,0},
      {3,27682,12433,3458,0,0,0,0,0,0,0,0,0},
      {3,312,4844,13138,0,0,0,0,0,0,0,0,0},
      {3,17715,35138,15456,0,0,0,0,0,0,0,0,0},
      {3,30507,33307,30783,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_11_20N[99][14]=
    {
      {13,20834,22335,21330,11913,6036,15830,11069,10539,4244,15068,7113,2704,16224},
      {13,2010,5628,27960,11690,22545,24432,4986,21083,17529,4104,11941,21239,9602},
      {13,689,13248,1777,4876,2537,20869,15718,9575,18164,5294,13914,21711,23374},
      {13,9675,21239,13600,24710,10613,14804,19412,23270,26741,10503,25258,17816,25210},
      {13,12518,8680,6422,22715,25097,26959,3913,26493,7797,25977,4896,27063,20781},
      {13,21715,12850,7963,4027,4295,14931,18158,616,20570,8720,16487,19050,23925},
      {13,7939,21089,15170,24325,6651,22352,5633,27903,2685,1310,5594,9296,25670},
      {13,25121,13906,8217,25390,9112,13945,9826,10844,11418,10724,11518,9280,9576},
      {13,25979,23644,16073,27407,3476,28057,4003,2279,17490,7558,9538,22115,20439},
      {13,20708,22572,14997,15780,5159,11356,10931,8514,23275,2560,912,15935,20703},
      {13,26467,17173,21964,15469,21967,10380,16222,15106,16786,19542,28560,18387,27909},
      {13,14897,6167,24295,1266,16902,9546,11628,12048,24495,3706,22629,14165,2333},
      {13,19403,18738,28140,13141,6151,22785,9620,4290,2342,4902,15856,19033,22820},
      {13,15761,1985,9160,4435,11164,5442,23572,6951,19077,15406,16658,18324,19229},
      {13,16997,10094,19982,22821,7810,19660,1182,21968,16564,17453,10780,17034,16405},
      {13,11,28611,10411,15799,15705,2773,28601,19333,19447,16790,4618,15841,23854},
      {13,24686,4131,1013,2141,6052,11896,18719,16813,22420,23406,21052,4333,17754},
      {13,16425,17614,26883,12101,8224,13979,6869,25215,25991,28968,19337,25361,20513},
      {13,1671,14990,20692,24951,19446,7163,4959,13197,19201,3883,22532,15468,11856},
      {13,22758,23586,16985,18396,7434,11817,363,11824,285,20897,16646,16095,17011},
      {13,25144,14916,6302,20972,25439,6156,21776,19701,27803,9695,12941,23541,27425},
      {13,6979,27910,7378,8983,6280,4134,28860,8079,20892,28776,7899,23399,87},
      {13,18045,23929,25876,15560,23629,18376,4053,14655,2450,11907,19535,28543,3513},
      {13,4704,16512,16554,14062,2596,10357,17316,1011,22090,11353,20300,15300,18536},
      {13,14293,4746,28831,20028,16742,16835,28405,11245,10802,20242,17737,9590,20693},
      {13,26547,22557,22517,6285,5336,3998,2351,6628,22949,1517,4712,1770,9207},
      {13,28522,14116,5455,13105,18709,3030,4217,6306,27448,1943,23866,20212,18857},
      {3,14794,21425,15659,0,0,0,0,0,0,0,0,0,0},
      {3,4446,21140,13454,0,0,0,0,0,0,0,0,0,0},
      {3,21115,3271,1443,0,0,0,0,0,0,0,0,0,0},
      {3,2153,12424,6159,0,0,0,0,0,0,0,0,0,0},
      {3,23559,22473,26065,0,0,0,0,0,0,0,0,0,0},
      {3,15914,22980,12766,0,0,0,0,0,0,0,0,0,0},
      {3,3482,16233,5719,0,0,0,0,0,0,0,0,0,0},
      {3,27020,12322,24014,0,0,0,0,0,0,0,0,0,0},
      {3,25438,26499,26506,0,0,0,0,0,0,0,0,0,0},
      {3,21987,16027,6832,0,0,0,0,0,0,0,0,0,0},
      {3,17330,2620,20756,0,0,0,0,0,0,0,0,0,0},
      {3,15985,10471,23302,0,0,0,0,0,0,0,0,0,0},
      {3,593,6869,27185,0,0,0,0,0,0,0,0,0,0},
      {3,22961,9129,25646,0,0,0,0,0,0,0,0,0,0},
      {3,10702,12334,23959,0,0,0,0,0,0,0,0,0,0},
      {3,6375,23299,26942,0,0,0,0,0,0,0,0,0,0},
      {3,8029,4072,24051,0,0,0,0,0,0,0,0,0,0},
      {3,15147,5113,14725,0,0,0,0,0,0,0,0,0,0},
      {3,1451,27291,28731,0,0,0,0,0,0,0,0,0,0},
      {3,18808,11561,249,0,0,0,0,0,0,0,0,0,0},
      {3,28962,21405,18944,0,0,0,0,0,0,0,0,0,0},
      {3,6889,3314,23457,0,0,0,0,0,0,0,0,0,0},
      {3,27708,14530,8795,0,0,0,0,0,0,0,0,0,0},
      {3,6185,28821,6550,0,0,0,0,0,0,0,0,0,0},
      {3,2259,17627,701,0,0,0,0,0,0,0,0,0,0},
      {3,20819,18831,20140,0,0,0,0,0,0,0,0,0,0},
      {3,4991,11369,4282,0,0,0,0,0,0,0,0,0,0},
      {3,13230,3413,27092,0,0,0,0,0,0,0,0,0,0},
      {3,14556,5068,16209,0,0,0,0,0,0,0,0,0,0},
      {3,4337,24652,498,0,0,0,0,0,0,0,0,0,0},
      {3,715,28883,2285,0,0,0,0,0,0,0,0,0,0},
      {3,16524,25513,26034,0,0,0,0,0,0,0,0,0,0},
      {3,21067,15122,21667,0,0,0,0,0,0,0,0,0,0},
      {3,27982,15280,3313,0,0,0,0,0,0,0,0,0,0},
      {3,7563,22779,22453,0,0,0,0,0,0,0,0,0,0},
      {3,4744,17277,27210,0,0,0,0,0,0,0,0,0,0},
      {3,19170,10806,18815,0,0,0,0,0,0,0,0,0,0},
      {3,26424,26442,7837,0,0,0,0,0,0,0,0,0,0},
      {3,26264,28931,6020,0,0,0,0,0,0,0,0,0,0},
      {3,4645,20678,13160,0,0,0,0,0,0,0,0,0,0},
      {3,18111,28045,23883,0,0,0,0,0,0,0,0,0,0},
      {3,5128,10876,3087,0,0,0,0,0,0,0,0,0,0},
      {3,28551,26276,3541,0,0,0,0,0,0,0,0,0,0},
      {3,20152,10181,28172,0,0,0,0,0,0,0,0,0,0},
      {3,26430,14769,6809,0,0,0,0,0,0,0,0,0,0},
      {3,4956,16130,11348,0,0,0,0,0,0,0,0,0,0},
      {3,1691,10216,5743,0,0,0,0,0,0,0,0,0,0},
      {3,7848,20236,2661,0,0,0,0,0,0,0,0,0,0},
      {3,10660,8321,6155,0,0,0,0,0,0,0,0,0,0},
      {3,2757,6963,2596,0,0,0,0,0,0,0,0,0,0},
      {3,27791,6707,258,0,0,0,0,0,0,0,0,0,0},
      {3,12785,21176,15450,0,0,0,0,0,0,0,0,0,0},
      {3,7477,17274,25201,0,0,0,0,0,0,0,0,0,0},
      {3,262,18996,15836,0,0,0,0,0,0,0,0,0,0},
      {3,5287,11970,13365,0,0,0,0,0,0,0,0,0,0},
      {3,3098,17823,10786,0,0,0,0,0,0,0,0,0,0},
      {3,21831,14476,11447,0,0,0,0,0,0,0,0,0,0},
      {3,1893,3625,25404,0,0,0,0,0,0,0,0,0,0},
      {3,20880,21987,1228,0,0,0,0,0,0,0,0,0,0},
      {3,20942,15045,21358,0,0,0,0,0,0,0,0,0,0},
      {3,18237,28914,15673,0,0,0,0,0,0,0,0,0,0},
      {3,24273,284,9803,0,0,0,0,0,0,0,0,0,0},
      {3,13949,15670,16693,0,0,0,0,0,0,0,0,0,0},
      {3,15553,27782,22644,0,0,0,0,0,0,0,0,0,0},
      {3,27980,24820,27733,0,0,0,0,0,0,0,0,0,0},
      {3,7015,20974,10016,0,0,0,0,0,0,0,0,0,0},
      {3,26164,20314,25916,0,0,0,0,0,0,0,0,0,0},
      {3,11489,13663,11777,0,0,0,0,0,0,0,0,0,0},
      {3,18230,11483,5655,0,0,0,0,0,0,0,0,0,0},
      {3,1618,19977,26521,0,0,0,0,0,0,0,0,0,0},
      {3,25639,13184,28994,0,0,0,0,0,0,0,0,0,0},
      {3,3821,18349,13846,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_26_45N[104][14]=
    {
      {13,12918,15296,894,10855,350,453,11966,1667,18720,12943,24437,8135,2834},
      {13,11861,3827,15431,8827,8253,23393,15048,5554,16297,2994,6727,19453,2371},
      {13,26414,3044,20240,18313,11618,3145,10976,5786,5609,16358,2547,11557,14755},
      {13,26434,2510,26719,4420,6753,917,7821,26765,11684,9811,5420,6653,19554},
      {13,11928,20579,17439,19103,21162,11235,19172,22254,3420,10558,3646,11858,24120},
      {13,10189,8172,5004,26082,4345,5139,15135,26522,6172,17492,8462,4392,4546},
      {13,27330,21498,13424,8077,10165,9739,482,23749,1515,12788,10464,9085,20875},
      {13,12009,22276,18401,7541,5871,23053,16979,16300,13566,19424,5293,18290,23917},
      {12,9613,24175,11374,11736,17676,13126,20931,20290,20659,2000,7969,9386,0},
      {12,21507,24494,11822,21771,26776,21175,27354,15815,7598,19809,611,10144,0},
      {12,195,14244,7229,13002,14328,17987,14595,6985,7642,9434,7079,5571,0},
      {12,10013,3641,14064,11716,4620,18119,23365,26446,26273,25164,11262,26019,0},
      {12,15166,19403,5606,20138,1893,645,5414,12097,18635,21648,12255,13269,0},
      {12,1895,9969,8372,17737,21679,17061,20219,2513,27199,11242,17025,1261,0},
      {12,12845,13086,16256,15177,20822,10862,18375,6751,17532,24725,6966,18489,0},
      {12,8373,25550,20688,16686,7894,24599,21578,12516,7115,4836,23473,25162,0},
      {12,14375,9150,6606,21633,16224,23708,20350,4575,143,13356,10239,22868,0},
      {12,10760,19807,7079,16382,26236,22606,16777,24312,16941,26684,8658,19279,0},
      {12,15136,8603,332,2898,21821,23778,3232,12052,14336,7832,5600,27015,0},
      {12,14392,26564,21616,8332,21750,10379,19730,7553,27352,2718,15202,25661,0},
      {12,6891,13210,15284,21940,8742,10965,3176,25034,25137,25161,13267,7012,0},
      {12,4993,9943,13260,20980,20224,20129,2120,23111,16640,23548,21445,10794,0},
      {12,4846,2858,22663,12584,20448,4629,17825,22269,11278,26312,9463,21085,0},
      {12,24282,18233,9220,14979,24106,14507,24838,19689,17589,7926,7893,21701,0},
      {12,12253,26122,8035,20823,2584,4703,25178,5460,4190,7057,1144,8426,0},
      {12,12354,7216,19484,4110,22105,1452,11457,12539,27106,14256,14113,20701,0},
      {12,2547,26926,25933,11919,12026,24639,19741,15457,9239,26713,22838,6051,0},
      {12,8782,14714,23363,450,19972,2622,19473,24182,2391,26205,10018,9202,0},
      {12,15690,10472,20263,469,18876,23660,9005,12595,23818,26430,926,6156,0},
      {12,5440,5209,14958,9882,18843,22063,12749,18473,22546,11768,4493,12833,0},
      {12,18540,3544,9471,15893,14761,23479,22010,15491,19608,25035,9094,24836,0},
      {12,15909,16594,23538,25136,25063,24995,5354,905,18580,15476,20710,7774,0},
      {3,6088,17133,11498,0,0,0,0,0,0,0,0,0,0},
      {3,4721,17594,18267,0,0,0,0,0,0,0,0,0,0},
      {3,1645,23638,26645,0,0,0,0,0,0,0,0,0,0},
      {3,14800,17920,22016,0,0,0,0,0,0,0,0,0,0},
      {3,12927,350,19391,0,0,0,0,0,0,0,0,0,0},
      {3,19447,19886,25992,0,0,0,0,0,0,0,0,0,0},
      {3,26120,1747,11234,0,0,0,0,0,0,0,0,0,0},
      {3,1588,23170,27232,0,0,0,0,0,0,0,0,0,0},
      {3,2230,15468,18709,0,0,0,0,0,0,0,0,0,0},
      {3,17410,11055,20645,0,0,0,0,0,0,0,0,0,0},
      {3,3244,25815,14204,0,0,0,0,0,0,0,0,0,0},
      {3,2858,7980,12780,0,0,0,0,0,0,0,0,0,0},
      {3,3256,20418,24355,0,0,0,0,0,0,0,0,0,0},
      {3,24260,16245,20948,0,0,0,0,0,0,0,0,0,0},
      {3,11122,1503,15651,0,0,0,0,0,0,0,0,0,0},
      {3,19272,24054,6075,0,0,0,0,0,0,0,0,0,0},
      {3,4905,931,18884,0,0,0,0,0,0,0,0,0,0},
      {3,23633,17244,6067,0,0,0,0,0,0,0,0,0,0},
      {3,5568,26403,490,0,0,0,0,0,0,0,0,0,0},
      {3,16113,16055,10524,0,0,0,0,0,0,0,0,0,0},
      {3,23013,8138,12876,0,0,0,0,0,0,0,0,0,0},
      {3,20699,20123,15435,0,0,0,0,0,0,0,0,0,0},
      {3,27272,27296,22638,0,0,0,0,0,0,0,0,0,0},
      {3,7658,17259,20553,0,0,0,0,0,0,0,0,0,0},
      {3,14914,17891,12137,0,0,0,0,0,0,0,0,0,0},
      {3,16323,1085,18895,0,0,0,0,0,0,0,0,0,0},
      {3,21503,17141,2915,0,0,0,0,0,0,0,0,0,0},
      {3,21979,23246,1271,0,0,0,0,0,0,0,0,0,0},
      {3,14409,11303,12604,0,0,0,0,0,0,0,0,0,0},
      {3,25591,12157,14704,0,0,0,0,0,0,0,0,0,0},
      {3,18739,19265,8140,0,0,0,0,0,0,0,0,0,0},
      {3,11244,5962,6647,0,0,0,0,0,0,0,0,0,0},
      {3,3589,6029,6489,0,0,0,0,0,0,0,0,0,0},
      {3,16416,185,9426,0,0,0,0,0,0,0,0,0,0},
      {3,1267,14086,22473,0,0,0,0,0,0,0,0,0,0},
      {3,17159,22404,23608,0,0,0,0,0,0,0,0,0,0},
      {3,7230,22514,21605,0,0,0,0,0,0,0,0,0,0},
      {3,7645,1239,10717,0,0,0,0,0,0,0,0,0,0},
      {3,12028,13404,12140,0,0,0,0,0,0,0,0,0,0},
      {3,14784,15425,14895,0,0,0,0,0,0,0,0,0,0},
      {3,26165,18980,15386,0,0,0,0,0,0,0,0,0,0},
      {3,14399,7725,14908,0,0,0,0,0,0,0,0,0,0},
      {3,8463,22853,22095,0,0,0,0,0,0,0,0,0,0},
      {3,5517,1854,8283,0,0,0,0,0,0,0,0,0,0},
      {3,24381,260,12595,0,0,0,0,0,0,0,0,0,0},
      {3,839,23743,22445,0,0,0,0,0,0,0,0,0,0},
      {3,13473,8017,7716,0,0,0,0,0,0,0,0,0,0},
      {3,8697,13050,16975,0,0,0,0,0,0,0,0,0,0},
      {3,26656,16911,11972,0,0,0,0,0,0,0,0,0,0},
      {3,26173,2504,15216,0,0,0,0,0,0,0,0,0,0},
      {3,7493,6461,12840,0,0,0,0,0,0,0,0,0,0},
      {3,4464,14912,3745,0,0,0,0,0,0,0,0,0,0},
      {3,21461,9734,25841,0,0,0,0,0,0,0,0,0,0},
      {3,4659,7599,9984,0,0,0,0,0,0,0,0,0,0},
      {3,17519,7389,75,0,0,0,0,0,0,0,0,0,0},
      {3,12589,9862,8680,0,0,0,0,0,0,0,0,0,0},
      {3,23053,21981,25299,0,0,0,0,0,0,0,0,0,0},
      {3,19246,3243,15916,0,0,0,0,0,0,0,0,0,0},
      {3,21733,4467,26491,0,0,0,0,0,0,0,0,0,0},
      {3,4959,10093,20074,0,0,0,0,0,0,0,0,0,0},
      {3,9140,15000,12783,0,0,0,0,0,0,0,0,0,0},
      {3,854,10701,25850,0,0,0,0,0,0,0,0,0,0},
      {3,13624,7755,10789,0,0,0,0,0,0,0,0,0,0},
      {3,3977,15812,10783,0,0,0,0,0,0,0,0,0,0},
      {3,5830,6774,10151,0,0,0,0,0,0,0,0,0,0},
      {3,21375,25110,5830,0,0,0,0,0,0,0,0,0,0},
      {3,15985,18342,2623,0,0,0,0,0,0,0,0,0,0},
      {3,4716,27211,18500,0,0,0,0,0,0,0,0,0,0},
      {3,18370,12487,7335,0,0,0,0,0,0,0,0,0,0},
      {3,4362,21569,16881,0,0,0,0,0,0,0,0,0,0},
      {3,10421,15454,13015,0,0,0,0,0,0,0,0,0,0},
      {3,5794,1239,9934,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_28_45N[112][12]=
    {
      {11,24402,4786,12678,6376,23965,10003,15376,15164,21366,24252,3353},
      {11,8189,3297,18493,17994,16296,11970,16168,15911,20683,11930,3119},
      {11,22463,11744,13833,8279,21652,14679,23663,4389,15110,17254,17498},
      {11,13616,426,18060,598,19615,9494,3987,8014,13361,4131,13185},
      {11,4176,17725,14717,3414,10033,17879,8079,12107,10852,1375,19459},
      {11,1450,4123,2111,17490,13209,8048,15285,4422,11667,18290,19621},
      {11,2067,15982,304,8658,19120,6746,13569,19253,2227,22778,23826},
      {11,11667,11145,20469,17485,13697,3712,4258,16831,22634,18035,7275},
      {11,23804,14496,17938,15883,14984,15944,2816,22406,22111,2319,14731},
      {11,8541,12579,22121,8602,16755,6704,23740,16151,20297,9633,1100},
      {11,19569,10549,19086,21110,11659,6901,21295,7637,11756,8293,9071},
      {11,9527,9135,7181,19534,2157,788,13347,17355,17509,711,20116},
      {11,21217,15801,12175,9604,17521,2127,21103,1346,8921,7976,3363},
      {11,11036,5152,19173,8086,3571,1955,4146,13309,15934,19132,5510},
      {11,12935,13966,15399,16179,8206,19233,16702,7127,12185,15420,1383},
      {11,6222,6384,20549,18914,23658,11189,638,9297,17741,9747,13598},
      {11,17209,11974,20776,2146,9023,3192,19646,3393,1727,15588,20185},
      {11,5008,3885,5035,15852,5189,13877,15177,3049,22164,16540,21064},
      {11,24004,10345,12255,36,24008,8764,13276,13131,2358,24010,16203},
      {11,21121,21691,8555,11918,129,8860,23600,3042,3949,19554,12319},
      {11,22514,11709,11874,11656,536,9142,3901,580,1547,10749,5529},
      {11,3324,6251,1156,112,13086,5373,5119,132,18069,10482,19519},
      {11,17279,2017,14846,21417,17154,21735,18788,11759,192,16027,6234},
      {11,20417,3788,15159,22188,21251,16633,13579,8128,1841,23554,15056},
      {7,12104,9182,6147,1553,12750,4071,6495,0,0,0,0},
      {7,4961,18460,23266,10785,10973,4405,2707,0,0,0,0},
      {7,7665,7043,1968,3589,15378,9642,21148,0,0,0,0},
      {7,13073,13298,20040,13582,17124,348,12055,0,0,0,0},
      {3,378,7476,9838,0,0,0,0,0,0,0,0},
      {3,15454,5218,14834,0,0,0,0,0,0,0,0},
      {3,17678,3445,18453,0,0,0,0,0,0,0,0},
      {3,2767,388,12638,0,0,0,0,0,0,0,0},
      {3,5688,56,6360,0,0,0,0,0,0,0,0},
      {3,20009,872,16872,0,0,0,0,0,0,0,0},
      {3,10206,5551,477,0,0,0,0,0,0,0,0},
      {3,10662,23689,19768,0,0,0,0,0,0,0,0},
      {3,8965,17535,4421,0,0,0,0,0,0,0,0},
      {3,19397,18734,5422,0,0,0,0,0,0,0,0},
      {3,10043,22104,21682,0,0,0,0,0,0,0,0},
      {3,508,1588,23853,0,0,0,0,0,0,0,0},
      {3,1092,7288,4358,0,0,0,0,0,0,0,0},
      {3,2283,22298,10504,0,0,0,0,0,0,0,0},
      {3,15022,8592,22291,0,0,0,0,0,0,0,0},
      {3,11844,17038,2983,0,0,0,0,0,0,0,0},
      {3,17404,14541,6446,0,0,0,0,0,0,0,0},
      {3,20724,7498,2993,0,0,0,0,0,0,0,0},
      {3,14715,9410,6844,0,0,0,0,0,0,0,0},
      {3,20213,14674,263,0,0,0,0,0,0,0,0},
      {3,4822,20951,635,0,0,0,0,0,0,0,0},
      {3,20651,23174,5057,0,0,0,0,0,0,0,0},
      {3,22237,9229,4859,0,0,0,0,0,0,0,0},
      {3,17280,9586,20334,0,0,0,0,0,0,0,0},
      {3,19508,8068,11375,0,0,0,0,0,0,0,0},
      {3,5776,21209,9418,0,0,0,0,0,0,0,0},
      {3,6872,6349,20397,0,0,0,0,0,0,0,0},
      {3,11165,19619,13108,0,0,0,0,0,0,0,0},
      {3,13550,10715,5122,0,0,0,0,0,0,0,0},
      {3,5655,10699,8415,0,0,0,0,0,0,0,0},
      {3,9864,4985,7986,0,0,0,0,0,0,0,0},
      {3,6436,3754,7690,0,0,0,0,0,0,0,0},
      {3,4257,17119,5328,0,0,0,0,0,0,0,0},
      {3,659,4687,6006,0,0,0,0,0,0,0,0},
      {3,527,10824,8234,0,0,0,0,0,0,0,0},
      {3,11291,1735,22513,0,0,0,0,0,0,0,0},
      {3,7254,2617,1493,0,0,0,0,0,0,0,0},
      {3,3015,7462,10953,0,0,0,0,0,0,0,0},
      {3,15705,2181,11992,0,0,0,0,0,0,0,0},
      {3,4628,19430,18223,0,0,0,0,0,0,0,0},
      {3,9426,21808,13549,0,0,0,0,0,0,0,0},
      {3,17008,3470,22568,0,0,0,0,0,0,0,0},
      {3,13643,24195,21816,0,0,0,0,0,0,0,0},
      {3,936,14226,22874,0,0,0,0,0,0,0,0},
      {3,6156,19306,18215,0,0,0,0,0,0,0,0},
      {3,23984,14714,12907,0,0,0,0,0,0,0,0},
      {3,5139,18639,15609,0,0,0,0,0,0,0,0},
      {3,11908,5446,8958,0,0,0,0,0,0,0,0},
      {3,6315,16864,15814,0,0,0,0,0,0,0,0},
      {3,10686,22570,16196,0,0,0,0,0,0,0,0},
      {3,203,4208,13716,0,0,0,0,0,0,0,0},
      {3,494,14172,11778,0,0,0,0,0,0,0,0},
      {3,15112,14244,8417,0,0,0,0,0,0,0,0},
      {3,21087,4602,15570,0,0,0,0,0,0,0,0},
      {3,19758,4401,22270,0,0,0,0,0,0,0,0},
      {3,8218,11940,5009,0,0,0,0,0,0,0,0},
      {3,23833,13785,12569,0,0,0,0,0,0,0,0},
      {3,1698,7113,18541,0,0,0,0,0,0,0,0},
      {3,18711,19991,19673,0,0,0,0,0,0,0,0},
      {3,8025,17107,14784,0,0,0,0,0,0,0,0},
      {3,5954,6817,19810,0,0,0,0,0,0,0,0},
      {3,24143,12236,18063,0,0,0,0,0,0,0,0},
      {3,23748,23956,10369,0,0,0,0,0,0,0,0},
      {3,7805,13982,13861,0,0,0,0,0,0,0,0},
      {3,5198,10889,6787,0,0,0,0,0,0,0,0},
      {3,10406,13918,3305,0,0,0,0,0,0,0,0},
      {3,12219,6523,12999,0,0,0,0,0,0,0,0},
      {3,9964,2004,17361,0,0,0,0,0,0,0,0},
      {3,23759,21507,11984,0,0,0,0,0,0,0,0},
      {3,4188,19754,13358,0,0,0,0,0,0,0,0},
      {3,8027,3662,2411,0,0,0,0,0,0,0,0},
      {3,19762,16017,9125,0,0,0,0,0,0,0,0},
      {3,2393,4619,5452,0,0,0,0,0,0,0,0},
      {3,24176,6586,10895,0,0,0,0,0,0,0,0},
      {3,15872,1795,15801,0,0,0,0,0,0,0,0},
      {3,6911,15300,14787,0,0,0,0,0,0,0,0},
      {3,2584,4905,8833,0,0,0,0,0,0,0,0},
      {3,1327,12862,9476,0,0,0,0,0,0,0,0},
      {3,16768,12633,7400,0,0,0,0,0,0,0,0},
      {3,11983,6276,18370,0,0,0,0,0,0,0,0},
      {3,12939,12793,20048,0,0,0,0,0,0,0,0},
      {3,20284,12949,21345,0,0,0,0,0,0,0,0},
      {3,19545,4503,16017,0,0,0,0,0,0,0,0},
      {3,1253,12068,18813,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_23_36N[115][12]=
    {
      {11,2475,3722,16456,6081,4483,19474,20555,10558,4351,4052,20066},
      {11,1547,5612,22269,11685,23297,19891,18996,21694,7927,19412,15951},
      {11,288,15139,7767,3059,1455,12056,12721,7938,19334,3233,5711},
      {11,6664,7486,17133,2931,20176,20158,9634,20002,13129,10015,13595},
      {11,218,22642,9357,11999,22898,4446,8059,1913,22365,10039,15203},
      {11,10305,22970,7928,16564,8402,9988,7039,10195,22389,5451,8731},
      {11,19073,1005,18826,11109,13748,11891,21530,15924,21128,6841,11064},
      {11,3240,11632,18386,22456,3963,14719,4244,4599,8098,7599,12862},
      {11,5666,11543,9276,19923,19171,19591,6005,8623,22777,1255,20078},
      {11,17064,13244,323,11349,6637,8611,6695,4750,20985,18144,5584},
      {11,20309,6210,16745,10959,14284,2893,20916,10985,9664,9065,11703},
      {11,17833,21598,22375,12890,10779,11241,13115,9222,21139,1217,15337},
      {11,15514,12517,18953,11458,17296,8751,7213,12078,4994,4391,14976},
      {11,3842,21548,10955,11679,16551,8514,17999,20557,16497,12122,23056},
      {11,10551,20186,66,11038,22049,2130,1089,22093,9069,3470,8079},
      {11,19208,22044,2732,1325,22309,967,22951,1366,11745,5556,6926},
      {11,2805,18271,10046,4277,207,19518,17387,9701,8515,6813,10532},
      {11,19714,21923,13493,1768,18819,6093,14086,13695,12781,9782,445},
      {11,22160,15778,13629,10312,19769,8567,22096,15558,19730,11861,18492},
      {11,10729,16847,273,4119,4392,11480,20396,3505,7220,390,5546},
      {6,17277,8531,17390,22364,7167,2217,0,0,0,0,0},
      {6,7325,3832,19899,21104,8400,3906,0,0,0,0,0},
      {6,6218,20330,14943,14477,5614,1582,0,0,0,0,0},
      {6,21534,14286,14624,14809,6775,22838,0,0,0,0,0},
      {6,15786,6527,15848,5288,13523,9692,0,0,0,0,0},
      {3,12696,15315,602,0,0,0,0,0,0,0,0},
      {3,17081,6828,13578,0,0,0,0,0,0,0,0},
      {3,3492,6510,20337,0,0,0,0,0,0,0,0},
      {3,6113,5090,7290,0,0,0,0,0,0,0,0},
      {3,20122,15539,19267,0,0,0,0,0,0,0,0},
      {3,10412,19090,17863,0,0,0,0,0,0,0,0},
      {3,2546,2295,19448,0,0,0,0,0,0,0,0},
      {3,20296,2296,2627,0,0,0,0,0,0,0,0},
      {3,6740,14224,10460,0,0,0,0,0,0,0,0},
      {3,12878,6055,15452,0,0,0,0,0,0,0,0},
      {3,15152,15699,563,0,0,0,0,0,0,0,0},
      {3,15414,21900,19161,0,0,0,0,0,0,0,0},
      {3,11126,15975,3733,0,0,0,0,0,0,0,0},
      {3,4379,15742,6475,0,0,0,0,0,0,0,0},
      {3,17203,5870,18537,0,0,0,0,0,0,0,0},
      {3,4912,260,21115,0,0,0,0,0,0,0,0},
      {3,23164,4273,1694,0,0,0,0,0,0,0,0},
      {3,1082,5287,11152,0,0,0,0,0,0,0,0},
      {3,14537,2277,19232,0,0,0,0,0,0,0,0},
      {3,13414,15608,12926,0,0,0,0,0,0,0,0},
      {3,17043,18241,18313,0,0,0,0,0,0,0,0},
      {3,208,6118,20777,0,0,0,0,0,0,0,0},
      {3,9140,19241,22845,0,0,0,0,0,0,0,0},
      {3,18527,5035,4161,0,0,0,0,0,0,0,0},
      {3,20867,22650,5585,0,0,0,0,0,0,0,0},
      {3,7875,10358,1898,0,0,0,0,0,0,0,0},
      {3,3563,14833,21329,0,0,0,0,0,0,0,0},
      {3,14705,3359,13959,0,0,0,0,0,0,0,0},
      {3,4507,11976,20017,0,0,0,0,0,0,0,0},
      {3,22424,12925,8308,0,0,0,0,0,0,0,0},
      {3,8739,15561,8010,0,0,0,0,0,0,0,0},
      {3,6408,20723,20928,0,0,0,0,0,0,0,0},
      {3,12337,7864,15777,0,0,0,0,0,0,0,0},
      {3,12742,20430,17351,0,0,0,0,0,0,0,0},
      {3,6259,1865,9808,0,0,0,0,0,0,0,0},
      {3,8343,17441,2551,0,0,0,0,0,0,0,0},
      {3,2167,3025,23181,0,0,0,0,0,0,0,0},
      {3,22718,13243,4797,0,0,0,0,0,0,0,0},
      {3,4223,4982,4395,0,0,0,0,0,0,0,0},
      {3,1609,16748,17625,0,0,0,0,0,0,0,0},
      {3,8463,15204,19632,0,0,0,0,0,0,0,0},
      {3,6583,9112,20284,0,0,0,0,0,0,0,0},
      {3,11334,19370,4763,0,0,0,0,0,0,0,0},
      {3,746,18560,15222,0,0,0,0,0,0,0,0},
      {3,8796,12725,15176,0,0,0,0,0,0,0,0},
      {3,10245,15567,9991,0,0,0,0,0,0,0,0},
      {3,17447,18373,21523,0,0,0,0,0,0,0,0},
      {3,1473,5286,15793,0,0,0,0,0,0,0,0},
      {3,17675,21170,6699,0,0,0,0,0,0,0,0},
      {3,15515,15942,8733,0,0,0,0,0,0,0,0},
      {3,7047,11348,14584,0,0,0,0,0,0,0,0},
      {3,20435,19603,1961,0,0,0,0,0,0,0,0},
      {3,18851,7069,11402,0,0,0,0,0,0,0,0},
      {3,19180,6487,2979,0,0,0,0,0,0,0,0},
      {3,2650,13282,9040,0,0,0,0,0,0,0,0},
      {3,22613,23266,4786,0,0,0,0,0,0,0,0},
      {3,20832,3001,23129,0,0,0,0,0,0,0,0},
      {3,3850,5255,6601,0,0,0,0,0,0,0,0},
      {3,19827,15438,13956,0,0,0,0,0,0,0,0},
      {3,15798,4430,11318,0,0,0,0,0,0,0,0},
      {3,4724,8719,21209,0,0,0,0,0,0,0,0},
      {3,18127,844,21379,0,0,0,0,0,0,0,0},
      {3,7427,22987,10233,0,0,0,0,0,0,0,0},
      {3,22949,8145,21778,0,0,0,0,0,0,0,0},
      {3,7622,14471,18874,0,0,0,0,0,0,0,0},
      {3,8566,14340,3381,0,0,0,0,0,0,0,0},
      {3,3373,419,11514,0,0,0,0,0,0,0,0},
      {3,15127,917,13136,0,0,0,0,0,0,0,0},
      {3,19375,18740,4951,0,0,0,0,0,0,0,0},
      {3,960,2856,17804,0,0,0,0,0,0,0,0},
      {3,662,8107,10298,0,0,0,0,0,0,0,0},
      {3,10993,11755,19142,0,0,0,0,0,0,0,0},
      {3,11400,18818,521,0,0,0,0,0,0,0,0},
      {3,7210,18658,8285,0,0,0,0,0,0,0,0},
      {3,9496,20836,5655,0,0,0,0,0,0,0,0},
      {3,14654,13694,12705,0,0,0,0,0,0,0,0},
      {3,20381,16473,7271,0,0,0,0,0,0,0,0},
      {3,12796,3280,23370,0,0,0,0,0,0,0,0},
      {3,13893,7667,1736,0,0,0,0,0,0,0,0},
      {3,5485,18321,7789,0,0,0,0,0,0,0,0},
      {3,11242,18771,17282,0,0,0,0,0,0,0,0},
      {3,817,21060,15985,0,0,0,0,0,0,0,0},
      {3,666,20461,22464,0,0,0,0,0,0,0,0},
      {3,7696,19774,4324,0,0,0,0,0,0,0,0},
      {3,12239,14014,4759,0,0,0,0,0,0,0,0},
      {3,5011,10472,4137,0,0,0,0,0,0,0,0},
      {3,3047,2444,3818,0,0,0,0,0,0,0,0},
      {3,1594,20382,538,0,0,0,0,0,0,0,0},
      {3,7051,21874,1697,0,0,0,0,0,0,0,0},
      {3,18539,26,21487,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_25_36N[125][12]=
    {
      {11,11863,9493,4143,12695,8706,170,4967,798,9856,6015,5125},
      {11,12288,19567,18233,15430,1671,3787,10133,15709,7883,14260,17039},
      {11,2066,12269,14620,7577,11525,19519,6181,3850,8893,272,12473},
      {11,8857,12404,1136,19464,15113,12598,12147,4987,13843,12152,13241},
      {11,1354,12339,4308,23,12677,11533,3187,11609,4740,14630,19630},
      {11,14508,10946,3928,580,3526,17836,3786,15739,13991,1238,1071},
      {11,6977,13222,13811,585,8154,2579,8314,12185,15876,7738,5691},
      {11,12901,12576,11597,4893,17238,15556,8106,12472,10455,14530,17432},
      {11,8373,12875,16582,14611,14267,15093,2405,9342,18326,12125,9257},
      {11,5861,12284,2441,13280,2762,5076,17758,4359,6156,18961,13208},
      {11,4400,8474,19629,19528,14125,12780,12740,19316,491,4761,1719},
      {11,7270,6615,1175,15848,6943,18360,8905,13921,10807,19688,18757},
      {11,8312,12234,17907,17254,7699,18399,5508,12215,4818,18107,2874},
      {11,19496,13973,10432,13445,15320,13648,1501,10549,6710,8897,1998},
      {11,1575,12713,10916,5316,13713,11318,4055,5782,5828,17981,3141},
      {11,12177,10726,4244,3138,15996,6822,7495,5257,8909,6180,10680},
      {11,6650,1909,19146,1038,17229,10050,3051,9793,10839,3532,14759},
      {11,5337,8448,4939,14792,7585,17860,8612,2229,18965,1519,2031},
      {11,13845,9320,579,15441,15050,752,8303,6989,13360,12927,15255},
      {11,17286,3639,1733,16883,8457,9475,2939,3234,1993,8554,9939},
      {11,6359,15474,12100,6992,13844,16988,7481,16977,9052,9262,15270},
      {11,7181,3624,3814,16379,182,4338,17627,3315,5745,14093,15574},
      {11,10709,18662,6909,11248,5268,412,5854,16782,16059,10498,5061},
      {11,13321,617,6734,3718,15441,19241,17214,1682,18641,18646,6330},
      {11,7377,16951,14477,6507,9922,11464,2563,5702,12691,10606,17874},
      {9,7198,12571,17617,4862,18899,7100,8130,9665,10779,0,0},
      {9,6789,11459,17651,3693,13332,3854,7737,12589,15189,0,0},
      {9,16260,14569,9442,17890,18097,6845,6960,1376,8099,0,0},
      {9,12719,14986,18999,14013,3449,13618,14807,265,1508,0,0},
      {9,11231,966,15957,8315,3384,2570,5700,10911,17372,0,0},
      {3,153,8445,19598,0,0,0,0,0,0,0,0},
      {3,7841,14806,54,0,0,0,0,0,0,0,0},
      {3,2492,14099,11718,0,0,0,0,0,0,0,0},
      {3,18608,4278,333,0,0,0,0,0,0,0,0},
      {3,59,3982,16986,0,0,0,0,0,0,0,0},
      {3,3494,12496,2775,0,0,0,0,0,0,0,0},
      {3,18320,10650,16234,0,0,0,0,0,0,0,0},
      {3,9739,16537,19706,0,0,0,0,0,0,0,0},
      {3,7587,19072,18775,0,0,0,0,0,0,0,0},
      {3,14133,12042,2922,0,0,0,0,0,0,0,0},
      {3,229,17958,15889,0,0,0,0,0,0,0,0},
      {3,5130,11029,271,0,0,0,0,0,0,0,0},
      {3,5122,7021,7067,0,0,0,0,0,0,0,0},
      {3,12258,16611,9245,0,0,0,0,0,0,0,0},
      {3,15493,15347,15939,0,0,0,0,0,0,0,0},
      {3,741,12055,2822,0,0,0,0,0,0,0,0},
      {3,12804,3480,5690,0,0,0,0,0,0,0,0},
      {3,18598,19273,16354,0,0,0,0,0,0,0,0},
      {3,2569,16771,13693,0,0,0,0,0,0,0,0},
      {3,15051,853,956,0,0,0,0,0,0,0,0},
      {3,12256,2756,15137,0,0,0,0,0,0,0,0},
      {3,15685,2802,16479,0,0,0,0,0,0,0,0},
      {3,14687,12470,3583,0,0,0,0,0,0,0,0},
      {3,15473,17781,867,0,0,0,0,0,0,0,0},
      {3,4843,6765,13122,0,0,0,0,0,0,0,0},
      {3,11287,3680,19101,0,0,0,0,0,0,0,0},
      {3,4609,11385,13470,0,0,0,0,0,0,0,0},
      {3,12353,6632,206,0,0,0,0,0,0,0,0},
      {3,10984,3116,1263,0,0,0,0,0,0,0,0},
      {3,9419,14455,19438,0,0,0,0,0,0,0,0},
      {3,9528,1808,435,0,0,0,0,0,0,0,0},
      {3,2238,12870,10119,0,0,0,0,0,0,0,0},
      {3,10868,8402,11111,0,0,0,0,0,0,0,0},
      {3,11081,7197,2667,0,0,0,0,0,0,0,0},
      {3,13780,10759,19722,0,0,0,0,0,0,0,0},
      {3,3768,3052,1836,0,0,0,0,0,0,0,0},
      {3,446,1642,12388,0,0,0,0,0,0,0,0},
      {3,16876,8398,14485,0,0,0,0,0,0,0,0},
      {3,7301,14815,13811,0,0,0,0,0,0,0,0},
      {3,5678,10419,14396,0,0,0,0,0,0,0,0},
      {3,1877,14384,12817,0,0,0,0,0,0,0,0},
      {3,19028,19589,6893,0,0,0,0,0,0,0,0},
      {3,8725,6346,676,0,0,0,0,0,0,0,0},
      {3,13611,12486,2054,0,0,0,0,0,0,0,0},
      {3,11203,14908,14692,0,0,0,0,0,0,0,0},
      {3,18139,5334,1253,0,0,0,0,0,0,0,0},
      {3,16233,9749,16946,0,0,0,0,0,0,0,0},
      {3,18885,4332,16306,0,0,0,0,0,0,0,0},
      {3,3862,10395,13871,0,0,0,0,0,0,0,0},
      {3,3747,8900,3381,0,0,0,0,0,0,0,0},
      {3,13367,14132,7220,0,0,0,0,0,0,0,0},
      {3,15095,4219,15869,0,0,0,0,0,0,0,0},
      {3,13519,18079,17541,0,0,0,0,0,0,0,0},
      {3,19012,13943,19471,0,0,0,0,0,0,0,0},
      {3,2221,5710,13711,0,0,0,0,0,0,0,0},
      {3,5185,3363,10195,0,0,0,0,0,0,0,0},
      {3,9580,17331,15360,0,0,0,0,0,0,0,0},
      {3,14387,7596,9614,0,0,0,0,0,0,0,0},
      {3,17336,6371,6030,0,0,0,0,0,0,0,0},
      {3,14629,10636,10159,0,0,0,0,0,0,0,0},
      {3,2402,9170,4321,0,0,0,0,0,0,0,0},
      {3,1040,5899,153,0,0,0,0,0,0,0,0},
      {3,7710,7637,13966,0,0,0,0,0,0,0,0},
      {3,10919,8535,3791,0,0,0,0,0,0,0,0},
      {3,1968,2567,4986,0,0,0,0,0,0,0,0},
      {3,4166,8744,17691,0,0,0,0,0,0,0,0},
      {3,540,10695,10019,0,0,0,0,0,0,0,0},
      {3,17710,1188,10821,0,0,0,0,0,0,0,0},
      {3,5858,17012,17389,0,0,0,0,0,0,0,0},
      {3,3083,17587,12682,0,0,0,0,0,0,0,0},
      {3,5354,9537,6807,0,0,0,0,0,0,0,0},
      {3,4964,15942,9653,0,0,0,0,0,0,0,0},
      {3,9000,17053,13291,0,0,0,0,0,0,0,0},
      {3,11685,8503,10777,0,0,0,0,0,0,0,0},
      {3,13919,18155,9877,0,0,0,0,0,0,0,0},
      {3,1625,15314,13879,0,0,0,0,0,0,0,0},
      {3,18520,7074,17061,0,0,0,0,0,0,0,0},
      {3,3748,2752,7298,0,0,0,0,0,0,0,0},
      {3,493,19163,14139,0,0,0,0,0,0,0,0},
      {3,2260,18339,10688,0,0,0,0,0,0,0,0},
      {3,8928,17695,10276,0,0,0,0,0,0,0,0},
      {3,7640,18547,3561,0,0,0,0,0,0,0,0},
      {3,11275,5297,13167,0,0,0,0,0,0,0,0},
      {3,19691,19542,15725,0,0,0,0,0,0,0,0},
      {3,11837,7273,11297,0,0,0,0,0,0,0,0},
      {3,17873,7840,19563,0,0,0,0,0,0,0,0},
      {3,8109,3811,18417,0,0,0,0,0,0,0,0},
      {3,17759,17623,13175,0,0,0,0,0,0,0,0},
      {3,10041,4152,2249,0,0,0,0,0,0,0,0},
      {3,18452,1450,19309,0,0,0,0,0,0,0,0},
      {3,9161,11651,4614,0,0,0,0,0,0,0,0},
      {3,11547,14058,639,0,0,0,0,0,0,0,0},
      {3,9384,3272,12368,0,0,0,0,0,0,0,0},
      {3,5898,2578,14635,0,0,0,0,0,0,0,0},
      {3,15963,6733,11048,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_13_18N[130][11]=
    {
      {10,2510,12817,11890,13009,5343,1775,10496,13302,13348,17880},
      {10,6766,16330,2412,7944,2483,7602,12482,6942,3070,9231},
      {10,16410,1766,1240,10046,12091,14475,7003,202,7733,11237},
      {10,15562,4695,13931,17100,11102,770,3848,4216,7132,10929},
      {10,16469,17153,8177,8723,12861,15948,2251,1500,11526,8590},
      {10,14813,3505,12654,1079,11736,6290,2299,17073,6330,5997},
      {10,390,16492,13989,1320,14600,7061,6583,458,894,1596},
      {10,8625,7644,1322,16647,15763,10439,8740,5529,2969,13893},
      {10,13425,13121,5344,8739,4953,7654,17848,9334,9533,2731},
      {10,12506,10992,8762,5395,6424,11688,3193,17601,14679,8204},
      {10,5466,15487,1642,6671,13557,4074,7182,4436,12398,12973},
      {10,1958,13041,6579,15984,3762,16633,6113,11509,7227,28},
      {10,17202,4813,14024,15099,2648,4476,2260,6507,9930,9232},
      {10,14186,14510,6818,7665,12708,2645,16687,13255,8239,15884},
      {10,1751,7847,17987,11410,3345,17133,17655,5027,1261,17191},
      {10,8056,4264,13915,8217,6118,8072,6278,6835,5038,15008},
      {10,13625,2999,5336,11687,13500,5723,13903,766,6293,155},
      {10,12316,14093,7372,16846,15357,9865,17869,1429,16681,202},
      {10,15062,1123,6454,17625,3213,39,1669,1770,13636,16555},
      {10,13053,7597,11481,1336,3343,11387,5463,17830,13741,5976},
      {10,1956,13509,1664,16867,8168,13421,17078,3285,17138,1572},
      {10,16711,1499,4805,13584,14759,2844,13110,7356,5850,8330},
      {10,6521,8528,14170,6681,16992,12867,14326,15227,4082,8595},
      {10,16176,8184,8572,1923,935,8900,13020,6812,9778,3391},
      {10,3946,4711,15314,15108,15634,4144,4372,9207,10715,1291},
      {10,16601,5864,10968,4724,9235,6988,3307,6515,7004,16328},
      {10,16217,4227,9735,15857,5003,2532,4451,8574,2149,6908},
      {10,9506,8949,12035,9701,3124,14295,8567,13614,5159,16746},
      {10,2418,8669,10921,5738,147,1004,2692,9065,12877,7559},
      {10,16706,8511,10314,3118,1219,7071,12376,538,2389,3297},
      {3,12492,10589,5791,0,0,0,0,0,0,0},
      {3,13528,1653,6618,0,0,0,0,0,0,0},
      {3,10485,1307,4102,0,0,0,0,0,0,0},
      {3,347,13580,4039,0,0,0,0,0,0,0},
      {3,523,10311,10540,0,0,0,0,0,0,0},
      {3,4183,6192,17159,0,0,0,0,0,0,0},
      {3,11458,6521,9632,0,0,0,0,0,0,0},
      {3,11594,15791,10384,0,0,0,0,0,0,0},
      {3,11654,126,11715,0,0,0,0,0,0,0},
      {3,6265,34,5091,0,0,0,0,0,0,0},
      {3,7271,13900,7588,0,0,0,0,0,0,0},
      {3,3960,11297,1612,0,0,0,0,0,0,0},
      {3,9857,4695,16399,0,0,0,0,0,0,0},
      {3,6423,2197,15040,0,0,0,0,0,0,0},
      {3,4219,5979,13959,0,0,0,0,0,0,0},
      {3,2959,578,8404,0,0,0,0,0,0,0},
      {3,4585,658,6474,0,0,0,0,0,0,0},
      {3,15900,11357,5249,0,0,0,0,0,0,0},
      {3,7414,8642,1151,0,0,0,0,0,0,0},
      {3,4130,9064,14537,0,0,0,0,0,0,0},
      {3,14517,1356,3748,0,0,0,0,0,0,0},
      {3,13865,12085,17295,0,0,0,0,0,0,0},
      {3,9530,5110,1570,0,0,0,0,0,0,0},
      {3,10862,8458,15322,0,0,0,0,0,0,0},
      {3,16355,1774,5270,0,0,0,0,0,0,0},
      {3,1229,11587,1632,0,0,0,0,0,0,0},
      {3,17039,787,4703,0,0,0,0,0,0,0},
      {3,11423,15388,6136,0,0,0,0,0,0,0},
      {3,8413,9703,13946,0,0,0,0,0,0,0},
      {3,4678,4072,16702,0,0,0,0,0,0,0},
      {3,6244,4690,7164,0,0,0,0,0,0,0},
      {3,7238,14169,5398,0,0,0,0,0,0,0},
      {3,8679,122,11593,0,0,0,0,0,0,0},
      {3,10954,15802,16427,0,0,0,0,0,0,0},
      {3,9413,6717,16406,0,0,0,0,0,0,0},
      {3,1027,17863,7836,0,0,0,0,0,0,0},
      {3,655,8827,10286,0,0,0,0,0,0,0},
      {3,4124,12599,12482,0,0,0,0,0,0,0},
      {3,12955,3121,15318,0,0,0,0,0,0,0},
      {3,8343,16634,6301,0,0,0,0,0,0,0},
      {3,13568,5056,9920,0,0,0,0,0,0,0},
      {3,1948,10,17395,0,0,0,0,0,0,0},
      {3,8550,131,2151,0,0,0,0,0,0,0},
      {3,15226,15994,13093,0,0,0,0,0,0,0},
      {3,10966,15412,2781,0,0,0,0,0,0,0},
      {3,13425,15831,5346,0,0,0,0,0,0,0},
      {3,2261,1067,6346,0,0,0,0,0,0,0},
      {3,6625,1966,13533,0,0,0,0,0,0,0},
      {3,10575,4483,5761,0,0,0,0,0,0,0},
      {3,14366,2019,14426,0,0,0,0,0,0,0},
      {3,16746,1450,4830,0,0,0,0,0,0,0},
      {3,13109,7358,7942,0,0,0,0,0,0,0},
      {3,15376,7284,14035,0,0,0,0,0,0,0},
      {3,14341,12625,3306,0,0,0,0,0,0,0},
      {3,9375,7529,1537,0,0,0,0,0,0,0},
      {3,13831,13447,4549,0,0,0,0,0,0,0},
      {3,15658,15299,8238,0,0,0,0,0,0,0},
      {3,4005,13264,9766,0,0,0,0,0,0,0},
      {3,4715,6285,15383,0,0,0,0,0,0,0},
      {3,1262,12883,15434,0,0,0,0,0,0,0},
      {3,11123,14975,3434,0,0,0,0,0,0,0},
      {3,5307,1112,16967,0,0,0,0,0,0,0},
      {3,12163,12009,3681,0,0,0,0,0,0,0},
      {3,9174,13153,10344,0,0,0,0,0,0,0},
      {3,13456,13197,9562,0,0,0,0,0,0,0},
      {3,1785,7549,15347,0,0,0,0,0,0,0},
      {3,663,9748,9436,0,0,0,0,0,0,0},
      {3,4961,11903,11574,0,0,0,0,0,0,0},
      {3,16248,6238,666,0,0,0,0,0,0,0},
      {3,11426,13748,14763,0,0,0,0,0,0,0},
      {3,14431,1443,2069,0,0,0,0,0,0,0},
      {3,2376,8154,14978,0,0,0,0,0,0,0},
      {3,13140,1289,9046,0,0,0,0,0,0,0},
      {3,1159,300,3319,0,0,0,0,0,0,0},
      {3,11510,7769,15877,0,0,0,0,0,0,0},
      {3,6430,14946,6856,0,0,0,0,0,0,0},
      {3,8868,15622,12458,0,0,0,0,0,0,0},
      {3,4867,6622,6850,0,0,0,0,0,0,0},
      {3,14721,11241,12760,0,0,0,0,0,0,0},
      {3,14233,9874,17682,0,0,0,0,0,0,0},
      {3,16677,13195,15086,0,0,0,0,0,0,0},
      {3,11155,7067,14160,0,0,0,0,0,0,0},
      {3,12741,14379,8922,0,0,0,0,0,0,0},
      {3,1930,17055,11752,0,0,0,0,0,0,0},
      {3,12361,6523,9568,0,0,0,0,0,0,0},
      {3,12165,5636,16011,0,0,0,0,0,0,0},
      {3,11389,4754,9916,0,0,0,0,0,0,0},
      {3,15903,15542,8301,0,0,0,0,0,0,0},
      {3,12073,4918,9754,0,0,0,0,0,0,0},
      {3,16544,17907,14814,0,0,0,0,0,0,0},
      {3,10839,1401,5107,0,0,0,0,0,0,0},
      {3,12320,1095,8592,0,0,0,0,0,0,0},
      {3,15088,6521,12015,0,0,0,0,0,0,0},
      {3,14802,3901,8920,0,0,0,0,0,0,0},
      {3,17932,2990,1643,0,0,0,0,0,0,0},
      {3,5102,3870,2045,0,0,0,0,0,0,0},
      {3,540,2643,2287,0,0,0,0,0,0,0},
      {3,5844,2482,9471,0,0,0,0,0,0,0},
      {3,10428,637,3629,0,0,0,0,0,0,0},
      {3,8814,7277,2678,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_7_9N[140][13]=
    {
      {12,13057,12620,2789,3553,6763,8329,3333,7822,10490,13943,4101,2556},
      {12,658,11386,2242,7249,5935,2148,5291,11992,3222,2957,6454,3343},
      {12,93,1205,12706,11406,9017,7834,5358,13700,14295,4152,6287,4249},
      {12,6958,2768,8087,1759,11889,4474,3925,4004,14392,8923,6962,4822},
      {12,6719,5436,1905,10228,5059,4892,12448,26,12891,10607,12210,10424},
      {12,8368,10667,9045,7694,13097,3555,4831,411,8539,6527,12753,11530},
      {12,4960,6647,13969,3556,9997,7898,2134,9931,3749,4305,11242,10410},
      {12,9125,9075,9916,12370,8720,6056,8128,5425,979,3421,5660,9473},
      {12,4348,11979,5985,395,11255,13878,7797,4962,13519,13323,7596,5520},
      {12,2852,8519,3022,9432,3564,9467,8569,12235,11837,5031,4246,2},
      {12,4081,3630,1619,2525,3773,11491,14076,9834,3618,2008,4694,6948},
      {12,7684,9642,5970,1679,13207,12368,262,7401,11471,2861,5620,4754},
      {12,7474,10418,1422,10960,13852,988,13465,6415,86,2432,7595,12239},
      {12,8539,11749,8794,6350,1947,13325,13061,7385,13017,2536,13121,15},
      {12,7944,13831,5126,9938,11758,335,980,9736,12143,5753,4533,10814},
      {12,10706,12618,6949,2684,4107,14388,11372,6321,13832,9190,2838,13860},
      {12,10830,1947,13803,3257,2677,406,8400,10536,12911,3629,251,9784},
      {12,13343,13304,301,801,6456,6351,6155,6763,3812,11337,8446,9306},
      {12,524,5573,503,10544,8990,673,2309,12376,466,11441,960,1557},
      {12,4403,3564,1732,13453,12054,8941,1383,12424,4347,9830,3553,5158},
      {5,2025,4282,4983,13553,10776,0,0,0,0,0,0,0},
      {5,11833,13099,5078,4420,3527,0,0,0,0,0,0,0},
      {5,1544,7474,2780,7749,4153,0,0,0,0,0,0,0},
      {5,11189,520,8463,12230,7712,0,0,0,0,0,0,0},
      {5,10409,13367,2604,2966,9248,0,0,0,0,0,0,0},
      {5,1412,420,3507,9818,7955,0,0,0,0,0,0,0},
      {5,1122,12483,9375,10232,9456,0,0,0,0,0,0,0},
      {5,2799,7033,10404,4495,12059,0,0,0,0,0,0,0},
      {5,2569,5970,6262,2199,8045,0,0,0,0,0,0,0},
      {5,11724,511,12693,12855,9597,0,0,0,0,0,0,0},
      {5,756,12900,13391,13623,10683,0,0,0,0,0,0,0},
      {5,2095,13479,1488,9469,11142,0,0,0,0,0,0,0},
      {5,13849,1356,10776,3530,9866,0,0,0,0,0,0,0},
      {5,13449,14225,2072,12772,9461,0,0,0,0,0,0,0},
      {5,6466,6181,6502,401,7439,0,0,0,0,0,0,0},
      {5,4631,1086,3062,11789,11811,0,0,0,0,0,0,0},
      {5,6788,14007,2270,14132,2764,0,0,0,0,0,0,0},
      {5,4643,10272,11316,2608,8511,0,0,0,0,0,0,0},
      {5,5221,9028,2736,7223,1051,0,0,0,0,0,0,0},
      {5,1974,2737,6739,13904,6156,0,0,0,0,0,0,0},
      {3,5,9082,3915,0,0,0,0,0,0,0,0,0},
      {3,2400,7195,3413,0,0,0,0,0,0,0,0,0},
      {3,606,221,8171,0,0,0,0,0,0,0,0,0},
      {3,4548,1267,5310,0,0,0,0,0,0,0,0,0},
      {3,12795,2160,8305,0,0,0,0,0,0,0,0,0},
      {3,10563,3507,12190,0,0,0,0,0,0,0,0,0},
      {3,6325,2499,9717,0,0,0,0,0,0,0,0,0},
      {3,9251,6046,13308,0,0,0,0,0,0,0,0,0},
      {3,11704,10834,11241,0,0,0,0,0,0,0,0,0},
      {3,4777,3774,11533,0,0,0,0,0,0,0,0,0},
      {3,12487,10365,6852,0,0,0,0,0,0,0,0,0},
      {3,58,2650,2027,0,0,0,0,0,0,0,0,0},
      {3,7248,13704,5573,0,0,0,0,0,0,0,0,0},
      {3,12777,7834,8561,0,0,0,0,0,0,0,0,0},
      {3,7906,8121,7774,0,0,0,0,0,0,0,0,0},
      {3,554,3105,6000,0,0,0,0,0,0,0,0,0},
      {3,11198,3586,10410,0,0,0,0,0,0,0,0,0},
      {3,9002,4094,11297,0,0,0,0,0,0,0,0,0},
      {3,12058,1037,13638,0,0,0,0,0,0,0,0,0},
      {3,1258,12917,11078,0,0,0,0,0,0,0,0,0},
      {3,2430,51,10276,0,0,0,0,0,0,0,0,0},
      {3,7841,9451,10236,0,0,0,0,0,0,0,0,0},
      {3,11045,1058,10352,0,0,0,0,0,0,0,0,0},
      {3,9629,9428,86,0,0,0,0,0,0,0,0,0},
      {3,8146,1255,3802,0,0,0,0,0,0,0,0,0},
      {3,10820,6337,4199,0,0,0,0,0,0,0,0,0},
      {3,9364,7723,1139,0,0,0,0,0,0,0,0,0},
      {3,438,6445,583,0,0,0,0,0,0,0,0,0},
      {3,2683,5358,10730,0,0,0,0,0,0,0,0,0},
      {3,8471,3061,13380,0,0,0,0,0,0,0,0,0},
      {3,3005,2840,4754,0,0,0,0,0,0,0,0,0},
      {3,8210,1814,11502,0,0,0,0,0,0,0,0,0},
      {3,8667,14258,5985,0,0,0,0,0,0,0,0,0},
      {3,8407,13336,10970,0,0,0,0,0,0,0,0,0},
      {3,6363,11715,5053,0,0,0,0,0,0,0,0,0},
      {3,104,13618,13817,0,0,0,0,0,0,0,0,0},
      {3,6562,4087,294,0,0,0,0,0,0,0,0,0},
      {3,1742,10528,4626,0,0,0,0,0,0,0,0,0},
      {3,6607,2692,1587,0,0,0,0,0,0,0,0,0},
      {3,11097,8361,2788,0,0,0,0,0,0,0,0,0},
      {3,13451,3541,823,0,0,0,0,0,0,0,0,0},
      {3,4060,13604,9816,0,0,0,0,0,0,0,0,0},
      {3,157,6106,1062,0,0,0,0,0,0,0,0,0},
      {3,8853,5159,4270,0,0,0,0,0,0,0,0,0},
      {3,9352,13164,2919,0,0,0,0,0,0,0,0,0},
      {3,7526,5174,12501,0,0,0,0,0,0,0,0,0},
      {3,12634,13077,5129,0,0,0,0,0,0,0,0,0},
      {3,5750,1568,6281,0,0,0,0,0,0,0,0,0},
      {3,269,5985,10973,0,0,0,0,0,0,0,0,0},
      {3,8518,9415,1028,0,0,0,0,0,0,0,0,0},
      {3,4722,13275,634,0,0,0,0,0,0,0,0,0},
      {3,12113,7104,7436,0,0,0,0,0,0,0,0,0},
      {3,12787,1032,5936,0,0,0,0,0,0,0,0,0},
      {3,3425,11526,10797,0,0,0,0,0,0,0,0,0},
      {3,784,9208,15,0,0,0,0,0,0,0,0,0},
      {3,11223,12849,4913,0,0,0,0,0,0,0,0,0},
      {3,10635,3553,8852,0,0,0,0,0,0,0,0,0},
      {3,11749,10619,3532,0,0,0,0,0,0,0,0,0},
      {3,4080,9831,9219,0,0,0,0,0,0,0,0,0},
      {3,6560,6049,6111,0,0,0,0,0,0,0,0,0},
      {3,1304,11770,12585,0,0,0,0,0,0,0,0,0},
      {3,13209,8589,11287,0,0,0,0,0,0,0,0,0},
      {3,2887,10699,14307,0,0,0,0,0,0,0,0,0},
      {3,4752,456,4073,0,0,0,0,0,0,0,0,0},
      {3,1175,13156,4894,0,0,0,0,0,0,0,0,0},
      {3,12756,3237,6279,0,0,0,0,0,0,0,0,0},
      {3,10125,7074,2344,0,0,0,0,0,0,0,0,0},
      {3,7533,7103,5226,0,0,0,0,0,0,0,0,0},
      {3,4000,4425,12173,0,0,0,0,0,0,0,0,0},
      {3,10056,5312,1599,0,0,0,0,0,0,0,0,0},
      {3,7445,8696,12533,0,0,0,0,0,0,0,0,0},
      {3,11509,14050,2483,0,0,0,0,0,0,0,0,0},
      {3,12405,2876,5033,0,0,0,0,0,0,0,0,0},
      {3,4512,4955,5627,0,0,0,0,0,0,0,0,0},
      {3,5572,5099,10987,0,0,0,0,0,0,0,0,0},
      {3,10665,404,3082,0,0,0,0,0,0,0,0,0},
      {3,2075,1583,13454,0,0,0,0,0,0,0,0,0},
      {3,5666,7228,524,0,0,0,0,0,0,0,0,0},
      {3,13290,7634,418,0,0,0,0,0,0,0,0,0},
      {3,9006,7368,4181,0,0,0,0,0,0,0,0,0},
      {3,9447,3674,8171,0,0,0,0,0,0,0,0,0},
      {3,9355,10211,9342,0,0,0,0,0,0,0,0,0},
      {3,12572,3681,3322,0,0,0,0,0,0,0,0,0},
      {3,3295,186,7491,0,0,0,0,0,0,0,0,0},
      {3,7926,212,5241,0,0,0,0,0,0,0,0,0},
      {3,5479,1654,8097,0,0,0,0,0,0,0,0,0},
      {3,5078,423,4817,0,0,0,0,0,0,0,0,0},
      {3,1357,12780,3664,0,0,0,0,0,0,0,0,0},
      {3,11900,402,13108,0,0,0,0,0,0,0,0,0},
      {3,299,7166,12008,0,0,0,0,0,0,0,0,0},
      {3,5750,3041,5618,0,0,0,0,0,0,0,0,0},
      {3,8357,1229,8884,0,0,0,0,0,0,0,0,0},
      {3,3713,8791,13375,0,0,0,0,0,0,0,0,0},
      {3,4390,6302,568,0,0,0,0,0,0,0,0,0},
      {3,1009,4440,10003,0,0,0,0,0,0,0,0,0},
      {3,1209,11978,11711,0,0,0,0,0,0,0,0,0},
      {3,1803,9838,13537,0,0,0,0,0,0,0,0,0},
      {3,11318,9750,12421,0,0,0,0,0,0,0,0,0},
      {3,2388,3021,7880,0,0,0,0,0,0,0,0,0},
      {3,7220,1062,6871,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_90_180N[90][19]=
    {
      {18,708,1132,2120,3208,3879,8320,11948,14185,15214,16594,17849,19766,23814,26175,27579,28052,31512,32029},
      {18,2720,2753,3716,6133,8020,8305,9429,10337,15503,19905,20127,21963,25624,27221,27907,27945,29833,30270},
      {18,4011,7807,11547,12782,13040,14599,14836,15218,17890,18922,19668,20267,20714,22151,24373,25261,26101,27627},
      {18,136,5341,7661,12779,13392,13922,14151,15054,16544,17232,17478,19895,22814,23820,25014,26346,27575,31803},
      {18,3456,3485,5839,8312,8423,9796,10018,11520,13336,15520,19928,22019,23144,25339,27406,28729,29527,31406},
      {18,1779,3634,3930,4138,5449,5924,6776,7658,8703,11542,13133,15086,16334,21876,23860,24281,28854,29557},
      {18,697,868,1345,6257,7400,8220,9761,11501,15828,16175,16865,17251,19298,21907,24033,24175,24497,30965},
      {18,991,1845,3173,5609,11275,12666,12903,14409,15359,17537,17923,19821,20323,21561,21663,23378,25371,28487},
      {18,446,3096,3604,3688,6864,7296,8128,9957,11568,13204,14502,16280,17655,19695,25953,28006,31006,31160},
      {18,3592,5443,5450,8875,10529,10721,15241,16485,16905,17980,19685,21639,21938,25038,25322,26073,27072,32305},
      {3,2539,11274,18981,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8099,17427,18613,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7872,12547,14776,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,17272,31146,31476,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12171,20651,28060,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5845,20532,24021,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2102,9627,12746,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4406,13397,16767,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7707,19199,20221,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10278,11526,13739,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8902,13337,25524,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5385,6939,15968,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1686,2985,18124,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,21285,22673,25722,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4833,4895,7657,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,14073,19518,27540,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2832,27137,32072,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8936,19641,24786,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1696,4408,9480,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3941,9228,25043,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1328,7460,25237,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11251,21361,23890,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10450,10762,26795,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1544,19244,22553,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9564,24279,27073,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12214,15608,30892,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6316,29252,30504,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3667,7784,26600,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11435,20278,31840,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7211,21620,23899,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,17193,18368,23536,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3750,18865,29121,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2088,7648,20893,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12231,28534,28933,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6316,14450,31885,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2672,8770,26920,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,17337,18624,26359,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3098,17939,27134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1084,24695,31846,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5110,9148,10203,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3943,19212,31745,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6,6061,29453,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2271,9151,27015,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,386,2747,26182,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,13129,15456,30698,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,126,10222,23935,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11008,17244,19711,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,17752,22103,31308,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11293,20670,23940,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11627,14829,19929,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2163,5918,23666,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,28627,28709,31369,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3161,3209,26227,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1597,25322,30792,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2083,15971,16193,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4795,10875,11668,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12984,28077,28863,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1851,9750,30222,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2252,8660,8984,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6764,8375,15896,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5074,16399,31725,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11507,15519,30828,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3196,7975,17614,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,477,11889,17748,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2420,2852,25451,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {16,3683,4741,6944,8199,8808,13142,14071,15830,17216,18589,20284,21652,22542,24994,25719,26187,0,0},
      {16,1534,4620,4899,6461,6737,9082,10679,11544,16118,20173,20662,21526,22646,24778,29790,30044,0,0},
      {16,2343,2547,5620,6523,8188,9029,14162,15517,24143,25078,25163,26616,28731,30201,30889,32034,0,0},
      {16,1876,4541,5147,9087,12853,12967,13887,16009,19722,20475,21245,21908,22547,25790,27330,27640,0,0},
      {16,1706,3168,6471,7382,10195,11568,11770,17719,19484,19572,20375,20470,23332,24372,30654,31230,0,0},
      {9,996,3297,3587,4313,12243,12456,17510,20492,29071,0,0,0,0,0,0,0,0,0},
      {9,7114,7312,7937,8379,8580,11514,13657,23774,24569,0,0,0,0,0,0,0,0,0},
      {9,98,600,745,1223,4298,6362,12544,21620,28786,0,0,0,0,0,0,0,0,0},
      {9,2585,4021,10785,11294,20707,25033,25465,26990,30713,0,0,0,0,0,0,0,0,0},
      {9,1895,4346,10404,16998,17073,24131,24891,26056,26444,0,0,0,0,0,0,0,0,0},
      {6,4265,8641,8937,13462,23815,26205,0,0,0,0,0,0,0,0,0,0,0,0},
      {6,1468,2578,3070,6258,8221,10414,0,0,0,0,0,0,0,0,0,0,0,0},
      {6,5186,8832,11589,25697,29629,32022,0,0,0,0,0,0,0,0,0,0,0,0},
      {6,15971,17493,18659,19082,22089,26779,0,0,0,0,0,0,0,0,0,0,0,0},
      {6,1597,1691,10499,13815,18943,27396,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_96_180N[96][21]=
    {
      {20,551,1039,1564,1910,3126,4986,5636,5661,7079,9384,9971,10460,11259,14150,14389,14568,14681,21772,27818,28671},
      {20,384,1734,1993,3890,4594,6655,7483,8508,8573,8720,10388,15541,17306,18411,18606,19048,19273,21492,21970,29495},
      {20,1104,2877,10668,11101,12647,13994,14598,15653,17265,18435,18848,18949,19209,19312,19414,19465,24927,26613,28809,28865},
      {20,1185,6439,6519,7790,8609,8826,9934,16363,16596,18088,18757,20318,20446,21123,23938,24235,25120,25469,26036,28043},
      {20,53,3630,4384,4619,7805,8822,12208,13312,14269,16435,17925,18079,18689,19042,21541,22729,26066,27666,28759,30107},
      {20,1926,2549,9816,10544,10980,12468,13026,15658,15670,15975,17200,22364,22775,23343,24289,24956,26230,28040,28348,29718},
      {20,1243,1673,4181,6080,7829,8259,9294,10556,10905,14071,18080,18203,18537,19707,24143,24442,25877,27072,29158,29690},
      {20,1834,2523,5973,6006,8054,8843,10141,11668,12956,13202,18659,21757,24258,24675,24779,25924,26980,27008,29229,29899},
      {20,3790,5716,7345,7381,9081,9679,13629,18038,19858,21248,21348,22251,24410,26790,27643,27955,27996,28271,29638,30198},
      {20,158,545,1178,5181,8585,9927,10721,11361,11640,12552,12579,14641,14928,15609,17388,20551,24516,26834,29850,30201},
      {14,1076,3011,5636,6947,7236,7511,10629,16795,20770,22796,22853,24219,28516,29151,0,0,0,0,0,0},
      {14,678,2553,3403,6515,7079,8767,10228,10791,10832,16113,18718,21328,25762,26925,0,0,0,0,0,0},
      {14,8536,8706,9471,9854,11186,12220,13261,14034,14897,25068,26338,26427,28784,29281,0,0,0,0,0,0},
      {14,2634,3562,11652,13206,15185,17051,19666,21058,23107,23869,24590,25311,25498,28891,0,0,0,0,0,0},
      {14,2440,4164,7040,7591,9321,9796,11026,12204,13478,17852,22183,25257,28756,28813,0,0,0,0,0,0},
      {12,390,2209,3095,4554,5755,12285,12314,14372,14957,15711,22946,27713,0,0,0,0,0,0,0,0},
      {12,207,418,3856,8719,11708,15353,20717,21639,23269,26732,27613,28334,0,0,0,0,0,0,0,0},
      {12,2805,3795,7105,8130,10330,13888,15132,16415,17093,23277,25191,27630,0,0,0,0,0,0,0,0},
      {12,1381,1955,3689,6290,6342,9573,13751,14633,16298,18206,24248,24893,0,0,0,0,0,0,0,0},
      {12,5991,8976,9763,12308,12566,15265,17068,21084,22475,24371,25171,28008,0,0,0,0,0,0,0,0},
      {3,8962,17060,22260,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1335,6691,14738,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4118,10315,23113,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4643,10970,28091,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1195,3683,26231,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7486,17403,22471,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7328,10110,19764,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4630,13732,28298,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6139,19386,26009,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,19712,20670,27993,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9317,18037,19324,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4422,4510,10290,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1396,22324,28601,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1404,5708,22352,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,14783,17214,19359,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7996,20133,20614,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6219,17582,24404,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2481,20505,28124,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4569,10863,28776,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,941,7516,11474,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,24878,27151,28125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9053,10186,28126,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1376,19327,26055,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5965,14239,16875,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,17434,18098,25044,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5785,14385,22243,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,15144,16981,25171,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,13671,25732,25961,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,95,20461,20558,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4321,19572,26175,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3637,17351,18683,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,18096,23926,27359,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7914,13217,23098,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9822,11342,26728,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7591,18615,28044,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,112,16897,19127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10087,18516,27292,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2762,3323,21677,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4533,20728,23071,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7164,7180,15097,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2061,6184,20598,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6310,13462,26714,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8189,9345,15315,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3319,17370,24726,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5217,9271,11984,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10245,13623,16621,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5537,22325,22692,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1592,19859,25578,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7005,15625,22572,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1474,14387,28592,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,153,4254,20080,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8709,25107,25135,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11663,17264,25598,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7135,17226,18698,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,109,2603,26360,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,1408,9608,11927,14872,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,4187,10410,27776,30125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,1008,4409,14938,20458,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,3641,12480,20141,22605,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,10453,22378,24993,30002,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,19957,24800,25196,29823,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,2431,2929,5824,12333,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,395,4393,15571,22019,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,15040,24712,28275,28794,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,3735,11988,13828,13949,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,4301,5829,24675,26696,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,6406,8140,9438,17274,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,4272,17638,19278,24961,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,3271,11030,11481,28624,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,3792,5394,7566,17557,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6505,11108,20811,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2722,28613,28991,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11472,25911,30170,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2355,13553,25490,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3284,13373,17330,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9046,16513,22243,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_100_180N[100][17]=
    {
      {16,690,1366,2591,2859,4224,5842,7310,8181,12432,15667,15717,16935,17583,19696,20573,21269},
      {16,2488,2890,6630,6892,11563,12518,15560,16798,18355,18746,19165,19295,21567,23505,23617,23629},
      {16,321,2844,2894,3986,4538,7599,7816,9831,10247,11556,16068,17249,18194,23043,23100,25938},
      {16,2503,2827,4771,5929,6400,7437,8054,10897,11633,14404,16133,17101,24425,24973,25086,25802},
      {16,1462,2099,3910,5131,5352,8832,9495,9624,10796,12906,13903,14724,14946,17708,21034,26612},
      {16,260,523,1427,3435,4517,9494,12594,12688,12726,14163,16537,17424,18424,20321,25101,28269},
      {16,2131,2407,4820,7167,11783,15249,15982,18761,22162,24593,24971,25831,26351,27005,28348,28793},
      {16,2089,5829,6119,7505,7758,8122,9870,12107,16656,17582,19115,23738,27646,27692,27862,28356},
      {16,2714,3288,3337,5013,6210,8080,9348,12919,13458,13621,18015,21024,24044,24761,25610,26317},
      {16,1305,3402,5830,7095,8852,9580,9793,11157,12725,14355,20659,21400,22289,23823,26250,27006},
      {3,12936,15702,23593,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3616,17219,18621,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1234,12759,26749,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,396,3017,18360,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10664,21597,26165,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12986,14553,24818,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,18403,21213,28302,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6515,18331,19413,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,19649,26219,27483,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2538,15793,17528,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7871,9374,20785,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5494,8084,21558,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6691,7770,14306,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3247,7417,18827,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11615,15987,20081,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1527,15272,26042,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10540,15548,23849,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,223,2601,25888,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2395,21465,28501,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,19703,21589,27252,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12832,15726,25300,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3750,10030,16124,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,401,6474,28682,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4424,19876,25563,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,590,12853,14779,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,25185,25539,25920,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6857,23329,25764,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3467,23205,23751,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9278,24364,25033,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,14956,19104,22487,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,21856,26163,27130,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2067,17357,22055,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,50,14414,19142,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,306,445,16437,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2260,13892,17608,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8893,12230,16916,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5943,8921,16380,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5079,15385,21951,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5108,6038,8513,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2126,6749,7330,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3814,11941,22949,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2301,15416,26731,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3498,14463,20417,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2062,10434,10746,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,18128,18960,23452,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,13080,13129,27193,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,18818,24995,27087,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7198,11948,23135,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,17206,18524,25811,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5202,10030,10076,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8497,23410,23991,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1553,1968,13135,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4426,10786,23259,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,92,7941,23045,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6356,14028,23104,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,18319,20286,22984,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5778,25335,26191,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,662,15922,27478,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2920,9733,18974,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,15337,27509,27519,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8659,25028,27723,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,14865,24564,26361,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1383,21234,21770,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10767,25752,25843,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7717,14536,24248,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {15,278,2803,2966,3547,4128,4829,4981,6699,6716,14183,14239,15939,16996,19694,20073,0},
      {15,3022,3087,10039,10174,11403,12146,13689,14934,17765,18121,18936,21818,27202,27532,28192,0},
      {15,817,3888,4102,9441,10165,10941,18131,20028,22305,23832,25225,26228,27208,27245,27390,0},
      {15,6346,7992,9053,11187,12124,16435,16850,21269,21580,22096,23700,24751,26264,27318,27576,0},
      {15,1440,3291,5755,12247,12272,15394,15659,15764,16338,17373,18840,19597,19812,22415,27062,0},
      {10,937,3118,8745,10933,12703,13906,14113,21442,21539,28140,0,0,0,0,0,0},
      {10,247,2465,2918,3189,5886,11451,16862,17458,20908,26608,0,0,0,0,0,0},
      {10,58,10104,11815,14429,16531,19797,24071,26021,28000,28489,0,0,0,0,0,0},
      {10,4367,5710,7855,14203,18071,19336,19880,20166,26774,28554,0,0,0,0,0,0},
      {10,191,1085,4068,7452,11739,15962,17501,19172,24130,28476,0,0,0,0,0,0},
      {8,4961,19716,19964,23479,24004,24340,25537,27930,0,0,0,0,0,0,0,0},
      {8,1688,2235,10464,15112,15134,25143,25910,28689,0,0,0,0,0,0,0,0},
      {8,765,11839,17427,19754,21445,22034,23493,25296,0,0,0,0,0,0,0,0},
      {8,277,7947,9952,12228,12595,16563,19758,21721,0,0,0,0,0,0,0,0},
      {8,1575,2652,5226,8159,16624,25446,26417,26722,0,0,0,0,0,0,0,0},
      {3,10571,17389,22602,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1331,7875,18475,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11738,13853,23914,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9412,11361,26507,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,16877,23022,27060,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2627,16649,22369,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9446,14752,28540,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4496,7705,22247,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2439,19741,28550,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6605,12623,26774,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_104_180N[104][19]=
    {
      {18,2087,6318,7314,8327,9453,12989,13156,13763,13819,16963,18495,19352,20510,20651,23379,23847,23953,26469},
      {18,2680,5652,6816,7854,10673,11431,12379,14570,17081,19341,20749,21056,22990,23012,24902,25547,26718,27284},
      {18,2142,3940,4724,4791,6617,6800,9349,9380,10073,10147,11750,12900,16044,16156,17769,21600,21669,22554},
      {18,1588,3097,4277,6181,6737,8974,9793,12215,12814,17953,18270,21808,22625,24390,25429,25750,25967,26391},
      {18,561,5825,7106,7166,7475,11844,12905,13559,13978,14176,14437,16070,16587,19792,20187,23754,26070,27232},
      {18,673,1783,4046,4887,5596,8390,9229,12315,14252,14415,14529,17837,20013,20032,22201,22487,24412,25792},
      {18,1261,1910,3767,6244,7050,7367,9230,12972,13229,13472,14287,14494,16776,20523,20738,21591,23622,25206},
      {18,1618,2106,3640,6304,7984,8158,9072,9311,12618,15746,16985,18923,20959,21267,23375,24052,24260,24827},
      {18,6256,6931,7276,7356,7832,12284,12405,13083,13602,14750,19021,20026,22661,23283,24427,25301,25982,27279},
      {18,2432,3076,3399,5305,7370,8406,8826,9237,10537,15492,15606,15619,16515,17562,19550,22525,24389,25740},
      {14,157,296,422,467,7125,9849,9997,15376,15506,16119,17153,17857,18639,23136,0,0,0,0},
      {14,1275,1439,6162,8258,9031,10207,10472,16004,16641,17140,21342,22191,23200,25753,0,0,0,0},
      {14,110,1073,6460,9208,10520,15833,15951,17494,18614,19970,20537,21512,21796,22135,0,0,0,0},
      {14,3771,5399,5885,7905,8302,8614,10205,11133,11459,16044,22701,25170,26255,27086,0,0,0,0},
      {14,1597,2640,2741,3790,5107,7470,9160,12078,12350,14020,18877,19507,22658,24290,0,0,0,0},
      {14,4957,5961,6263,8201,8579,9392,10133,11712,14757,15678,15718,19528,25107,25122,0,0,0,0},
      {14,870,4508,5944,7360,11724,15003,16387,19543,19893,20189,21942,23740,25686,25849,0,0,0,0},
      {14,131,2044,6731,7619,7787,9109,9841,10006,10275,13687,16522,18212,24457,25197,0,0,0,0},
      {14,504,1863,4246,5075,5448,6296,6930,11792,13736,14588,16340,17102,17807,26621,0,0,0,0},
      {14,1137,1168,2366,3818,4311,6806,8583,10850,12198,12357,21357,23243,23568,25003,0,0,0,0},
      {3,2353,11886,22548,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1680,9112,12175,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,15126,16642,27030,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5571,5719,19190,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6232,13413,19728,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8197,12068,17122,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3220,3476,24534,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1630,4345,23890,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,19815,20676,24443,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12761,14080,15937,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,41,7963,23895,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7658,13020,27128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1017,1447,3285,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2268,22921,26255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,261,13889,14175,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,13925,18777,18987,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,15136,24523,27156,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12008,18484,19299,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4304,9857,15134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2966,9278,9737,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5469,15449,22259,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11359,14186,20635,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,16453,21262,23629,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5613,7100,11104,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3959,14714,18116,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7465,13803,24660,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3939,7615,9891,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12249,16491,22373,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8734,14253,25616,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5781,18719,23894,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6208,6703,14626,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1284,4730,23920,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3920,13167,13366,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3925,7147,27268,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1926,12777,21092,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,675,8186,22557,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,487,9590,12433,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7090,16031,27037,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3083,10445,22950,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,380,4663,7195,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,960,12754,20597,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1790,12476,24250,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11307,22121,22266,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3256,7625,12046,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11034,11800,17383,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6142,14781,19944,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2679,11106,22783,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7769,11060,15178,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7384,9851,20205,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,14813,19271,22600,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3085,11637,19934,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6518,7995,19382,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11070,15498,26380,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,248,16291,23824,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4989,19436,26642,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {7,5954,16039,16042,20349,21326,24656,25427,0,0,0,0,0,0,0,0,0,0,0},
      {7,2558,6628,9167,16825,19069,20808,22617,0,0,0,0,0,0,0,0,0,0,0},
      {7,317,13859,14069,16104,18835,20040,26633,0,0,0,0,0,0,0,0,0,0,0},
      {7,2866,4153,5875,11698,15287,19719,25808,0,0,0,0,0,0,0,0,0,0,0},
      {7,536,6955,9735,16098,20694,24675,26881,0,0,0,0,0,0,0,0,0,0,0},
      {4,25,7316,9961,21037,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,7823,19458,20404,25186,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,7142,11057,17748,24788,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,11315,12358,21583,21836,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,8995,9326,12826,25981,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,2281,10560,10674,19801,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,5001,6655,26231,26542,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,800,15131,18482,22621,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,9060,12257,24786,25188,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,3462,17201,18960,24462,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,17631,26360,26425,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12774,20967,21391,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,14701,20696,26807,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5931,13144,14022,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,128,16460,26300,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,801,9487,25937,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6153,11296,23054,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2749,14434,20049,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1732,7646,20402,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3839,11031,26022,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2159,20918,21407,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,285,13785,24234,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1977,3899,7972,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4120,19101,23719,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_116_180N[116][19]=
    {
      {18,3880,4377,6147,6219,7873,8180,9157,10311,10862,15393,16522,17318,17609,18398,19290,19293,20296,22244},
      {18,1056,1647,5119,5201,6991,10038,10843,11614,11901,12026,14631,16749,16772,16915,17331,19235,19877,22763},
      {18,501,2634,2812,3085,3242,4952,5087,8334,8838,8993,12601,12849,13142,13852,14416,14444,15122,20692},
      {18,343,1183,5708,6798,6951,9154,9160,9508,9884,11874,11984,13737,14933,17208,21253,21822,22723,22898},
      {18,3332,4384,5137,8527,8749,10414,10536,12759,14769,16121,19255,19326,20283,20352,20629,20827,21226,22087},
      {18,60,3866,3895,4116,5631,6985,7205,7681,10031,12825,14266,14644,16396,17010,20221,20268,21729,21752},
      {18,61,1112,1392,1826,1910,4370,5910,6660,6943,7859,9628,10213,10701,12615,14453,17123,18667,20688},
      {18,880,2397,2669,7767,9683,9705,10430,13995,15972,16217,17187,18246,18869,21077,21884,21897,21927,22475},
      {18,748,1029,1446,2912,6784,6926,7710,11674,12014,12409,12438,14411,14723,15953,16020,17496,18230,19547},
      {18,1151,2295,2975,3082,6502,8269,9130,9629,10018,10235,14871,15834,17396,17777,19138,21871,22035,22927},
      {12,650,789,4632,4777,5004,8796,13479,14917,16826,16926,19144,20754,0,0,0,0,0,0},
      {12,1693,4906,5777,5907,6472,9792,11824,16134,16406,16440,18395,22338,0,0,0,0,0,0},
      {12,5172,5920,7987,9381,10573,11382,11512,13074,15755,16591,19518,20968,0,0,0,0,0,0},
      {12,1409,2508,6542,8993,10660,13691,14724,15597,19641,20809,21160,22767,0,0,0,0,0,0},
      {12,895,1446,3298,4018,5250,6269,8897,9049,12052,15311,16199,20677,0,0,0,0,0,0},
      {10,1,774,1248,2362,7019,8430,14321,14444,19664,21475,0,0,0,0,0,0,0,0},
      {10,1714,1973,4155,7536,7975,9323,9997,10627,20959,21824,0,0,0,0,0,0,0,0},
      {10,586,1907,2153,5914,7407,8311,8900,10060,18502,18818,0,0,0,0,0,0,0,0},
      {10,805,1897,3019,7404,10055,11779,11982,15319,21802,21913,0,0,0,0,0,0,0,0},
      {10,5276,5470,8725,11080,11939,17057,17960,18930,19814,22546,0,0,0,0,0,0,0,0},
      {3,1227,10140,18999,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,849,17266,18364,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4436,6167,14942,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11103,14219,19204,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6738,10043,20614,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1885,3173,13934,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2088,11344,20627,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2668,6722,20336,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11274,18439,21280,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2223,15960,21282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6555,7521,11051,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9037,11912,22911,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12952,19885,21298,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,13696,16793,17228,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1040,4501,6170,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1025,4522,21287,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1213,3817,12857,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1392,6601,12468,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,835,16504,19633,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,634,16014,19619,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6166,17343,21067,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6583,16107,18382,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5481,9653,18543,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,14634,15406,16179,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1952,7810,16892,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2271,12635,20456,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8838,10469,20629,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11400,16788,18756,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,230,11373,17104,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,17204,17733,20707,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8465,13092,22087,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8684,8983,10130,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11468,13469,21366,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9342,10115,19130,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3184,9535,11802,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,13495,16231,19609,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8911,12617,15190,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,508,8674,19422,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4059,6197,8449,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9440,11028,13468,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1779,9358,13459,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,46,7370,15160,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12118,17458,21853,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,320,4449,20048,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12300,14502,21803,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9019,19417,22280,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1320,6434,7916,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6850,10275,17099,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,301,5637,7309,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8443,13673,16625,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4943,15268,20252,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,13246,17809,18271,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3230,8329,12330,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1398,7959,18145,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,274,10500,12954,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {10,1326,2057,5453,6588,11514,11920,13687,14692,17684,22043,0,0,0,0,0,0,0,0},
      {10,3921,7217,8693,10943,11769,12121,12618,19781,19932,20083,0,0,0,0,0,0,0,0},
      {10,2166,5206,5482,11453,13986,16877,18184,18822,21663,22611,0,0,0,0,0,0,0,0},
      {10,858,11727,13116,14705,15517,16109,17086,18439,19047,20321,0,0,0,0,0,0,0,0},
      {10,216,414,726,2616,6948,7028,8288,12164,21697,22606,0,0,0,0,0,0,0,0},
      {4,7441,14498,15308,17321,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,1455,6627,10112,13652,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,7448,7945,17043,21758,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,2947,7933,11624,14237,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,514,4014,20406,22226,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,4454,9815,11696,13946,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,7787,11797,13113,15796,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,2363,4379,21733,22277,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,8437,16504,16527,21350,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,8932,14444,15680,19635,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1273,11365,15443,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3533,11221,13249,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,687,1927,14403,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3914,4221,8791,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12479,15622,17384,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,14928,20923,22283,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7729,13750,15716,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,88,12409,19522,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6852,16166,21884,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1204,12049,16487,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11178,11226,15971,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6382,14361,16863,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10195,10247,18188,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1819,5834,8434,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,286,3333,21431,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,13950,15188,17771,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10198,14887,16751,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,13811,18307,18337,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1210,18076,21869,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5717,8482,11896,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6501,15625,17792,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3965,4494,20272,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1589,9900,14472,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,288,9421,12009,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2177,4626,16605,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,710,4696,18127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_124_180N[124][17]=
    {
      {16,1083,2862,3815,4075,5519,8003,9308,10029,12476,12949,13759,13918,14303,15028,19737,19953},
      {16,392,3781,6086,8378,9952,10531,11369,11954,14808,14948,16585,16682,18445,18960,19085,19423},
      {16,3023,3727,4797,5104,5342,5994,8138,9758,10146,11758,14763,15300,15640,17947,18690,18864},
      {16,854,1259,2147,3861,4258,4949,5555,5940,9454,14940,15521,16393,17029,18789,18810,19840},
      {16,4404,6608,7232,7534,7721,8079,8558,9851,11560,11968,12678,13410,15908,16918,18108,18437},
      {16,519,1591,1600,1964,7706,9481,10789,11068,13056,13373,13759,14323,14999,15505,17366,18254},
      {16,545,673,2257,4060,4289,4897,5287,7318,8511,13835,14540,14948,15475,16718,17907,18067},
      {16,1949,3426,3440,4679,5103,8692,8987,10075,10923,11162,11625,12805,13749,17487,17807,18802},
      {16,858,1969,2178,2421,2592,2764,3504,7084,9227,9349,9960,10592,18149,18974,19010,19891},
      {16,3282,5061,5908,6929,7551,7927,8116,8388,11305,11379,12527,13982,14343,15064,16259,19602},
      {3,3730,8198,8789,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1515,6545,9678,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12411,14800,17119,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1000,15382,18607,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,977,1525,5376,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4464,7676,8937,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3684,6730,9836,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10203,10305,18629,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2214,4904,10873,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,690,7077,12623,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3094,11228,16285,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2583,5278,16500,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4253,13495,14465,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3323,17768,19548,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7670,12892,18704,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,373,14868,16337,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8886,17314,17578,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10636,12385,19530,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5734,14030,18619,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3298,4905,10156,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,332,19282,19924,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,15,8741,16429,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11482,14807,15426,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6055,12144,14026,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1095,5737,10525,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,813,965,4520,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,808,8546,14057,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3195,3814,14683,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1184,17287,19477,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12411,13207,18549,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2639,12198,15656,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3527,5555,14387,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5563,10402,19122,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4538,13134,18766,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,731,3368,5865,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1253,2238,8820,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2764,11942,16705,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6375,18789,19594,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3387,11299,14192,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2486,2729,8580,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3096,5778,10416,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2513,10609,14018,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2508,10361,15415,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5368,6612,17415,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1998,5687,17627,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2711,16277,17350,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5213,5820,9217,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5744,17794,19180,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9095,15302,19441,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10031,12094,18856,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,739,6709,11785,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1496,10418,15753,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9437,11589,19552,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7484,9656,12381,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2371,7237,7794,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,748,7213,9835,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1930,6418,8683,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5482,15289,18623,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10791,15731,18781,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3622,5730,14230,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1169,9420,19665,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10170,13288,14142,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3855,7239,18843,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,816,16956,19454,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3179,5689,16584,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {13,4313,6450,8761,11594,13187,14029,14509,14944,16947,17850,18270,18390,19934,0,0,0},
      {13,1680,2214,3859,3994,4276,6984,12261,13869,14696,16303,16467,16756,19754,0,0,0},
      {13,433,1009,3169,6825,7128,7294,7327,8175,16653,16657,17314,18598,19472,0,0,0},
      {13,1473,2110,2777,5217,5902,7136,7797,8650,9116,11267,14270,15342,18291,0,0,0},
      {13,349,2892,4148,7493,10142,13920,14079,14423,15229,16255,16808,18248,18362,0,0,0},
      {13,5879,7078,7457,9493,10771,11010,12068,12994,13007,13109,17983,19531,20087,0,0,0},
      {13,483,804,993,1078,1822,4646,4658,5458,8116,8443,10056,13440,18939,0,0,0},
      {13,490,865,1309,4339,6434,8210,9248,10588,13094,16476,17620,19378,19708,0,0,0},
      {13,163,899,2396,4212,6157,9036,11116,13295,13928,15111,16312,18369,19470,0,0,0},
      {13,985,1298,3213,5145,6917,7136,7183,10870,11329,12295,13466,14989,17909,0,0,0},
      {12,89,582,812,1761,5157,6039,7843,8418,8747,11575,13169,14160,0,0,0,0},
      {12,1871,2701,3252,7399,7646,9785,11274,17041,17361,18899,19430,19691,0,0,0,0},
      {12,1328,2165,2722,4120,4132,9855,10802,14441,16771,17679,18611,18718,0,0,0,0},
      {12,1166,3128,8585,9843,10411,12365,14141,15156,16987,17484,17702,19204,0,0,0,0},
      {12,943,952,4108,4832,6706,9245,14304,16528,17055,17698,18419,19526,0,0,0,0},
      {3,1340,7429,17768,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10358,12400,16483,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1070,4760,10051,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6992,8645,9886,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,756,7962,17532,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,13063,17794,18323,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,630,9881,20052,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5786,7779,15441,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5049,5860,16575,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10021,13811,20097,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2167,6374,19993,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1412,4441,11765,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,14750,17242,18319,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,507,1756,18791,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2277,6901,9690,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,14828,15959,16658,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4687,6452,16770,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,465,11415,13696,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,13370,15379,16190,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2988,12683,16796,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6382,14227,14295,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,17221,18167,18379,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9656,9841,10968,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,16917,19014,19869,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,15255,15400,17505,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6403,15345,16248,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6794,15772,18005,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3252,12230,12246,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9062,9082,10245,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,405,9373,19195,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5987,6006,6026,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2865,2887,2896,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,14889,14898,14924,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7791,7800,7809,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_128_180N[128][16]=
    {
      {15,790,1010,1064,2157,2569,3499,4637,4951,6789,8177,9888,10800,13254,13829,17946},
      {15,597,693,862,900,4750,4897,5410,5441,6491,8815,11894,13411,13696,14103,18413},
      {15,903,2779,2996,6100,7489,7560,8637,8853,10078,11372,12040,15911,16944,17059,17771},
      {15,1761,2084,2099,2232,3114,3120,7062,10527,10823,11945,13918,16359,17110,17654,18370},
      {15,677,1080,2329,5100,5106,6261,6383,10953,11968,12186,13266,14451,16092,17760,17871},
      {15,1069,3672,5304,6102,6907,8087,9477,9654,11581,14650,14808,14920,15397,16179,18595},
      {15,327,1161,2624,4494,4516,5555,6928,7455,7959,8734,8933,9753,10614,16263,17689},
      {15,1922,1932,6481,7488,7722,8836,10326,10633,11184,12555,13485,14155,16373,17486,18331},
      {15,1056,1624,1991,3585,6052,7838,10123,11470,14228,15146,16166,16390,17630,17679,17774},
      {15,295,3429,3587,4597,5017,5105,5136,5827,7308,8266,9632,11612,14879,16167,18491},
      {12,1523,1615,3368,6253,8510,9070,10020,10368,10718,11882,12014,15077,0,0,0},
      {12,421,3234,4290,4808,4983,9992,12569,13331,14457,15853,15943,18318,0,0,0},
      {12,583,2081,4320,6268,6284,9084,9638,10941,13335,15062,17310,17667,0,0,0},
      {12,573,5180,5758,5813,9655,9892,10763,11209,11717,14760,14972,16395,0,0,0},
      {12,151,1917,4190,5573,5629,6725,9653,9974,10008,11971,15132,18170,0,0,0},
      {12,132,1270,3074,7215,7878,8266,11875,12274,13134,15084,17120,17556,0,0,0},
      {12,845,2417,2435,5875,7758,7807,12521,13907,16400,17778,18260,18370,0,0,0},
      {12,2848,4714,5924,6507,7595,8422,9281,13140,13276,14589,15269,15943,0,0,0},
      {12,278,931,1186,3588,4072,6943,8429,9348,9863,10056,14376,15846,0,0,0},
      {12,3480,3887,4932,5888,10246,10281,11065,11434,12290,12345,12635,13917,0,0,0},
      {3,528,14523,18426,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4127,5613,9647,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8777,15790,18168,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3491,5608,10216,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5154,8811,16363,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,437,2834,3470,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9675,12773,17150,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2456,7748,8623,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3758,14333,18097,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3969,17136,18610,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6745,13708,18656,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6152,10273,13316,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7822,14888,15541,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,15501,16598,18531,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2497,8828,15453,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3443,6899,7293,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3721,13954,15822,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,719,13335,15342,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1566,7588,8362,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8644,13389,17476,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1513,8257,15942,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2620,7224,15557,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7696,12178,17371,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5285,8439,11367,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4961,7657,17125,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11382,11542,16823,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2429,7538,10992,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,680,7651,10178,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6794,11231,18328,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1195,12837,15710,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1156,17454,18260,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6541,10062,17436,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,70,282,7519,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,608,1919,7299,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3339,11187,15788,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4771,12599,13753,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1822,4233,10561,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5233,14135,15888,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4109,14837,18717,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3011,15644,17342,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10668,11462,15065,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2486,6822,7486,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3851,6182,11215,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,595,11064,15525,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9738,10045,14128,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,929,2222,11949,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10950,12273,15503,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3672,6760,9589,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3583,5887,8907,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,13351,15134,17291,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7770,9928,12542,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,268,10496,17937,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1318,2938,6971,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,428,1791,9729,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6895,8896,10420,0,0,0,0,0,0,0,0,0,0,0,0},
      {12,2946,4619,6209,7377,7931,8740,9223,12171,12985,13795,14141,16233,0,0,0},
      {12,217,958,995,3144,5905,6178,6596,10427,15013,15669,16343,18465,0,0,0},
      {12,357,2579,4550,5223,5890,7642,7900,8441,13416,17740,18131,18679,0,0,0},
      {12,894,1776,1852,3262,5830,6008,7877,9570,15838,16029,16176,16583,0,0,0},
      {12,2190,2698,3277,4748,5575,6822,8356,9692,11406,11697,12991,15275,0,0,0},
      {4,9695,12587,15112,17987,0,0,0,0,0,0,0,0,0,0,0},
      {4,5221,5710,15272,17606,0,0,0,0,0,0,0,0,0,0,0},
      {4,3068,9034,11853,17189,0,0,0,0,0,0,0,0,0,0,0},
      {4,2503,7618,9336,15768,0,0,0,0,0,0,0,0,0,0,0},
      {4,2069,2258,7450,10219,0,0,0,0,0,0,0,0,0,0,0},
      {4,778,8645,12173,12429,0,0,0,0,0,0,0,0,0,0,0},
      {4,6960,9073,12411,15065,0,0,0,0,0,0,0,0,0,0,0},
      {4,3515,5848,12776,15706,0,0,0,0,0,0,0,0,0,0,0},
      {4,4725,5967,15682,17350,0,0,0,0,0,0,0,0,0,0,0},
      {4,12416,14871,16503,18679,0,0,0,0,0,0,0,0,0,0,0},
      {3,4218,13329,17613,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,752,6184,9180,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3188,3971,11453,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2580,17414,18001,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10285,13728,15896,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,612,10652,12442,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7637,7724,15724,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1427,15130,15355,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,77,5271,8934,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3121,10373,11930,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11913,12253,15701,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6582,9841,10243,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11595,16319,16332,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6402,11107,14899,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4593,5442,9630,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1321,3877,17467,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1335,10771,12311,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,24,16695,18578,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11396,17644,18618,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7306,14777,15287,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1809,5769,10827,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,137,3555,5186,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,201,3340,10470,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8954,12160,17867,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6744,9179,14780,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3280,9637,17720,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1867,10642,14613,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4292,7451,14924,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1621,13335,16834,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8289,14826,15302,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3610,12151,12159,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3968,3976,5492,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3491,14734,17314,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3774,8427,10437,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3128,4202,15889,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3821,9781,10862,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8264,9191,12337,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1476,8123,8946,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_132_180N[132][16]=
    {
      {15,214,632,923,3251,6036,6570,8258,9462,10399,11781,12778,14807,15369,16105,17153},
      {15,652,1565,3710,3720,4603,7139,7817,9076,11532,13729,14362,15379,15488,15541,15777},
      {15,98,130,520,622,1806,2474,3378,4773,6896,7533,7744,11317,11511,11574,15853},
      {15,95,1168,2985,4995,5032,5310,6932,8571,9181,9193,9896,10257,12336,12811,14754},
      {15,1178,1969,2935,3432,3628,4814,5218,5676,6214,9953,10421,11091,13574,13772,15191},
      {15,2356,7171,8062,8652,8801,9917,10037,10164,10671,10975,13460,15594,15936,16378,16711},
      {15,1401,3622,4493,5190,6908,7193,9583,10283,11832,12152,12609,13343,13427,13839,15591},
      {15,485,4930,7552,7574,7629,8514,10870,10888,11614,11774,12414,13159,15555,16874,16897},
      {15,203,711,1373,5149,7271,8191,8523,9012,9645,11391,13989,14402,15572,16490,16985},
      {15,1020,3606,4996,5016,7632,9959,11098,11792,12649,12859,13381,14579,16204,16899,17030},
      {12,3653,4711,4777,4779,5203,8250,10671,12130,12449,13340,14148,14853,0,0,0},
      {12,3209,4098,4415,4777,5358,6681,8049,9805,10139,15608,15628,16532,0,0,0},
      {12,37,279,2890,3692,5680,7136,10862,11442,13688,14572,14978,16246,0,0,0},
      {12,150,2430,2659,3909,8619,9432,12372,12720,13213,14635,15316,15727,0,0,0},
      {12,759,7725,8548,10772,10897,11392,12273,13619,14465,14488,16191,17039,0,0,0},
      {12,499,2346,4909,4998,6555,10631,12374,13539,13954,14728,14768,16213,0,0,0},
      {12,286,458,1072,1982,3593,4541,5803,7260,7681,10279,15178,15701,0,0,0},
      {12,683,850,1430,4534,4990,9870,10385,10508,12633,13516,14763,15297,0,0,0},
      {12,1304,1620,2788,4431,8333,10080,11887,11994,12430,12578,15816,16317,0,0,0},
      {12,1020,2376,3071,4752,7844,12085,12338,12790,13930,14874,16264,16947,0,0,0},
      {3,2917,14555,16711,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7491,9627,11576,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,863,2959,15686,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3115,3698,4721,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1992,6059,9232,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6038,7185,14573,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1340,3421,3694,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4609,8628,12390,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2208,8716,8858,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,13808,15922,16148,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2249,11974,16896,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5853,13225,13788,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,815,4711,6530,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2209,2468,14725,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4410,5415,13854,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6355,6825,15280,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,309,9537,16469,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8068,13746,14396,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9323,10747,15016,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6903,8218,11652,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,680,3121,8534,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7311,10942,15810,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,877,965,6600,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1742,5760,12311,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3137,4854,11102,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2422,7181,7657,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11818,13570,15583,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6318,13695,13717,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3866,5279,6931,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10864,15450,15719,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4540,7389,17179,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4951,15064,16397,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7605,10323,11651,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4137,6356,7204,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5439,10310,14018,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12843,13416,14274,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2804,9644,10370,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11150,13275,14293,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5134,5240,11495,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,864,2151,13541,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,736,13561,17218,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8287,13491,16780,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5718,15660,16593,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8455,13981,15971,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9467,14810,16276,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2229,3946,8111,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7217,7241,12272,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,67,3678,5473,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6684,10779,16599,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9465,12372,16208,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6794,14248,16412,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2214,10815,11926,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3021,6374,12487,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3950,6042,9573,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7939,11686,14299,0,0,0,0,0,0,0,0,0,0,0,0},
      {12,350,3529,4079,4256,5849,7190,8860,10139,10232,10819,11381,14147,0,0,0},
      {12,317,992,2421,3962,4699,6659,7506,10225,10422,10631,12471,17133,0,0,0},
      {12,1042,1396,2353,2995,3377,5431,5872,6507,6958,8392,10521,15036,0,0,0},
      {12,2799,3188,3338,4212,5257,6667,7299,8647,9365,9822,15393,16334,0,0,0},
      {12,1095,1357,1964,2027,3439,5975,7077,10182,11538,12085,14873,15081,0,0,0},
      {4,5063,15980,16044,16895,0,0,0,0,0,0,0,0,0,0,0},
      {4,2675,3343,8369,15958,0,0,0,0,0,0,0,0,0,0,0},
      {4,186,10209,12824,14269,0,0,0,0,0,0,0,0,0,0,0},
      {4,4306,6720,10338,16589,0,0,0,0,0,0,0,0,0,0,0},
      {4,2260,7944,10926,16496,0,0,0,0,0,0,0,0,0,0,0},
      {4,821,2379,3453,11530,0,0,0,0,0,0,0,0,0,0,0},
      {4,818,3049,7651,16046,0,0,0,0,0,0,0,0,0,0,0},
      {4,2127,3717,10120,15916,0,0,0,0,0,0,0,0,0,0,0},
      {4,3267,11412,13437,15833,0,0,0,0,0,0,0,0,0,0,0},
      {4,1386,7706,15875,16377,0,0,0,0,0,0,0,0,0,0,0},
      {3,508,11392,13620,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4097,14269,15322,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9921,12311,12914,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7184,10571,15214,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3917,8952,11193,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1241,11798,14788,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10457,14430,14892,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5603,14302,16388,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,427,2770,6440,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9317,10050,14671,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3199,5089,5353,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7239,7411,13299,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,306,1674,14551,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,816,7484,12448,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,706,13444,15695,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,554,4597,9489,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2104,6359,12468,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9266,10617,11381,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3277,3793,6604,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1731,1887,9707,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,885,5432,7884,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1786,8137,13590,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5024,6886,16155,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2777,7172,8568,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3551,8533,13805,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3299,8732,15678,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,633,9789,14366,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11345,14813,16179,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1216,5414,13845,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5832,7474,10047,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1074,3156,9228,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4090,7614,10391,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2624,5520,13591,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3462,12548,12556,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2027,11569,14106,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1821,3571,8001,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3979,7285,9173,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11161,12334,16935,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2642,8811,8819,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5359,11128,13310,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,200,6362,9809,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1174,8836,13549,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_135_180N[135][15]=
    {
      {14,15,865,1308,2887,6202,6440,7201,9014,10015,10041,11780,13602,14265,15506},
      {14,1054,1416,2903,3746,3753,7608,9121,11097,11761,12334,14304,15284,15489,15860},
      {14,388,942,2207,2627,3453,6684,7105,8633,9292,9413,11574,11789,12990,13558},
      {14,896,1802,2100,4497,6650,7324,7693,11232,11848,12625,12982,13238,13719,15260},
      {14,2165,2313,3930,5231,9263,10942,12167,12938,13529,13806,14792,15118,15304,15970},
      {14,286,951,1083,3401,5527,10235,10417,10717,12204,12522,12974,13623,13645,13721},
      {14,895,2500,3051,4393,4686,5972,7932,8025,8731,9744,10323,10619,14961,16054},
      {14,1631,2304,3149,3453,4133,4459,5442,7051,8622,10051,10791,11566,12754,14016},
      {14,2747,4371,5647,5694,5899,8378,8965,9917,10472,12163,13349,14760,15005,16196},
      {14,1119,3605,4141,4969,6694,7031,7748,8800,9268,9982,10605,11768,12185,12254},
      {11,2825,3983,3991,6410,8249,8457,8770,9183,12028,12213,12448,0,0,0},
      {11,604,1720,2373,2919,7212,7867,7967,8044,10466,13860,14417,0,0,0},
      {11,301,1594,5664,9711,9763,10804,10816,11681,11842,12204,15041,0,0,0},
      {11,47,555,1796,2032,3923,5175,5204,7322,12008,15192,15273,0,0,0},
      {11,2564,2827,4053,4355,5383,6611,7951,10231,10605,12712,15035,0,0,0},
      {11,2243,3129,5665,9703,9913,10101,10212,13549,14952,15661,15696,0,0,0},
      {11,204,775,3771,5704,7007,7095,7543,9656,12426,12882,15545,0,0,0},
      {11,4224,4480,4521,5860,5993,6200,6810,8966,13588,13658,14863,0,0,0},
      {11,528,2425,4261,6534,9097,9746,10277,11570,11722,12614,14288,0,0,0},
      {11,3612,4482,6901,8453,8546,9588,10302,11161,11365,14894,15018,0,0,0},
      {3,3742,10567,16199,0,0,0,0,0,0,0,0,0,0,0},
      {3,7133,9058,11953,0,0,0,0,0,0,0,0,0,0,0},
      {3,6322,6923,15941,0,0,0,0,0,0,0,0,0,0,0},
      {3,8088,9184,10475,0,0,0,0,0,0,0,0,0,0,0},
      {3,677,2998,15174,0,0,0,0,0,0,0,0,0,0,0},
      {3,4761,5594,9851,0,0,0,0,0,0,0,0,0,0,0},
      {3,2307,13609,15098,0,0,0,0,0,0,0,0,0,0,0},
      {3,4022,12283,12286,0,0,0,0,0,0,0,0,0,0,0},
      {3,5993,8346,11208,0,0,0,0,0,0,0,0,0,0,0},
      {3,3910,15175,15351,0,0,0,0,0,0,0,0,0,0,0},
      {3,6964,10422,13372,0,0,0,0,0,0,0,0,0,0,0},
      {3,6125,13835,14292,0,0,0,0,0,0,0,0,0,0,0},
      {3,6234,7466,8536,0,0,0,0,0,0,0,0,0,0,0},
      {3,4785,6567,8576,0,0,0,0,0,0,0,0,0,0,0},
      {3,6743,10561,14130,0,0,0,0,0,0,0,0,0,0,0},
      {3,1735,7324,11459,0,0,0,0,0,0,0,0,0,0,0},
      {3,3414,5686,12861,0,0,0,0,0,0,0,0,0,0,0},
      {3,5457,13085,14942,0,0,0,0,0,0,0,0,0,0,0},
      {3,2789,9707,10189,0,0,0,0,0,0,0,0,0,0,0},
      {3,3474,7428,8655,0,0,0,0,0,0,0,0,0,0,0},
      {3,393,4691,5694,0,0,0,0,0,0,0,0,0,0,0},
      {3,4825,8033,15186,0,0,0,0,0,0,0,0,0,0,0},
      {3,1458,4367,5864,0,0,0,0,0,0,0,0,0,0,0},
      {3,5843,11855,14660,0,0,0,0,0,0,0,0,0,0,0},
      {3,7748,8189,15486,0,0,0,0,0,0,0,0,0,0,0},
      {3,4810,13665,13848,0,0,0,0,0,0,0,0,0,0,0},
      {3,5818,6651,8375,0,0,0,0,0,0,0,0,0,0,0},
      {3,693,5872,7945,0,0,0,0,0,0,0,0,0,0,0},
      {3,172,11594,12687,0,0,0,0,0,0,0,0,0,0,0},
      {3,5430,12616,15658,0,0,0,0,0,0,0,0,0,0,0},
      {3,6934,7909,11246,0,0,0,0,0,0,0,0,0,0,0},
      {3,3637,12306,12362,0,0,0,0,0,0,0,0,0,0,0},
      {3,3471,5213,9689,0,0,0,0,0,0,0,0,0,0,0},
      {3,14049,14507,15642,0,0,0,0,0,0,0,0,0,0,0},
      {3,2668,3016,15757,0,0,0,0,0,0,0,0,0,0,0},
      {3,3740,7546,12925,0,0,0,0,0,0,0,0,0,0,0},
      {3,6446,14217,15795,0,0,0,0,0,0,0,0,0,0,0},
      {3,6834,12517,16183,0,0,0,0,0,0,0,0,0,0,0},
      {3,6930,8193,10296,0,0,0,0,0,0,0,0,0,0,0},
      {3,4279,5463,11460,0,0,0,0,0,0,0,0,0,0,0},
      {3,197,1031,3531,0,0,0,0,0,0,0,0,0,0,0},
      {3,9722,9899,11678,0,0,0,0,0,0,0,0,0,0,0},
      {3,2962,7375,15462,0,0,0,0,0,0,0,0,0,0,0},
      {3,181,2986,13487,0,0,0,0,0,0,0,0,0,0,0},
      {3,908,3631,15042,0,0,0,0,0,0,0,0,0,0,0},
      {3,3273,8070,10865,0,0,0,0,0,0,0,0,0,0,0},
      {3,4099,6682,15571,0,0,0,0,0,0,0,0,0,0,0},
      {3,2864,6393,12639,0,0,0,0,0,0,0,0,0,0,0},
      {3,6486,7891,14560,0,0,0,0,0,0,0,0,0,0,0},
      {3,10819,11213,13405,0,0,0,0,0,0,0,0,0,0,0},
      {3,71,6734,8450,0,0,0,0,0,0,0,0,0,0,0},
      {3,3467,5016,15956,0,0,0,0,0,0,0,0,0,0,0},
      {3,6267,10180,15388,0,0,0,0,0,0,0,0,0,0,0},
      {3,1625,2994,13339,0,0,0,0,0,0,0,0,0,0,0},
      {3,2517,4489,7357,0,0,0,0,0,0,0,0,0,0,0},
      {11,27,454,1440,1874,2627,6126,8518,9120,10144,13007,13892,0,0,0},
      {11,439,991,5000,5256,7215,8109,8680,10694,12078,13454,15221,0,0,0},
      {11,1162,4754,5101,5940,8304,10907,11008,11528,13514,13837,15230,0,0,0},
      {11,1230,1618,2547,2922,5619,7415,12126,12406,14378,15306,15408,0,0,0},
      {11,140,446,3378,3908,6904,7783,8587,10420,10630,12602,13597,0,0,0},
      {4,1004,3374,7234,9291,0,0,0,0,0,0,0,0,0,0},
      {4,8358,8550,8658,11681,0,0,0,0,0,0,0,0,0,0},
      {4,3669,7500,8610,10360,0,0,0,0,0,0,0,0,0,0},
      {4,4892,9971,11846,13233,0,0,0,0,0,0,0,0,0,0},
      {4,329,1735,2397,13639,0,0,0,0,0,0,0,0,0,0},
      {4,12658,12789,14985,15091,0,0,0,0,0,0,0,0,0,0},
      {4,8580,8769,9451,15185,0,0,0,0,0,0,0,0,0,0},
      {4,2383,3542,4270,8840,0,0,0,0,0,0,0,0,0,0},
      {4,1379,2256,2452,15454,0,0,0,0,0,0,0,0,0,0},
      {4,1457,6459,7332,12968,0,0,0,0,0,0,0,0,0,0},
      {3,5323,7398,14302,0,0,0,0,0,0,0,0,0,0,0},
      {3,6056,9938,10618,0,0,0,0,0,0,0,0,0,0,0},
      {3,104,6041,12225,0,0,0,0,0,0,0,0,0,0,0},
      {3,4895,14004,14522,0,0,0,0,0,0,0,0,0,0,0},
      {3,1942,6495,6826,0,0,0,0,0,0,0,0,0,0,0},
      {3,8262,15584,16179,0,0,0,0,0,0,0,0,0,0,0},
      {3,11604,12644,12874,0,0,0,0,0,0,0,0,0,0,0},
      {3,3538,9506,15206,0,0,0,0,0,0,0,0,0,0,0},
      {3,666,6038,8853,0,0,0,0,0,0,0,0,0,0,0},
      {3,5941,8753,12449,0,0,0,0,0,0,0,0,0,0,0},
      {3,6500,8604,16045,0,0,0,0,0,0,0,0,0,0,0},
      {3,7937,12018,12966,0,0,0,0,0,0,0,0,0,0,0},
      {3,8164,14164,14528,0,0,0,0,0,0,0,0,0,0,0},
      {3,867,6180,10192,0,0,0,0,0,0,0,0,0,0,0},
      {3,3403,5208,10213,0,0,0,0,0,0,0,0,0,0,0},
      {3,1752,7835,11867,0,0,0,0,0,0,0,0,0,0,0},
      {3,1576,6993,11260,0,0,0,0,0,0,0,0,0,0,0},
      {3,2245,8237,14506,0,0,0,0,0,0,0,0,0,0,0},
      {3,1284,1807,5480,0,0,0,0,0,0,0,0,0,0,0},
      {3,9778,10034,13115,0,0,0,0,0,0,0,0,0,0,0},
      {3,8398,13975,15705,0,0,0,0,0,0,0,0,0,0,0},
      {3,6906,7770,8242,0,0,0,0,0,0,0,0,0,0,0},
      {3,1896,3277,10631,0,0,0,0,0,0,0,0,0,0,0},
      {3,2168,6889,8036,0,0,0,0,0,0,0,0,0,0,0},
      {3,1616,6908,11754,0,0,0,0,0,0,0,0,0,0,0},
      {3,11353,13863,14389,0,0,0,0,0,0,0,0,0,0,0},
      {3,2514,7212,12887,0,0,0,0,0,0,0,0,0,0,0},
      {3,5661,6511,10622,0,0,0,0,0,0,0,0,0,0,0},
      {3,4690,8892,10754,0,0,0,0,0,0,0,0,0,0,0},
      {3,12200,12486,14850,0,0,0,0,0,0,0,0,0,0,0},
      {3,4663,15405,15949,0,0,0,0,0,0,0,0,0,0,0},
      {3,302,309,1904,0,0,0,0,0,0,0,0,0,0,0},
      {3,5265,7100,7105,0,0,0,0,0,0,0,0,0,0,0},
      {3,4996,7928,11084,0,0,0,0,0,0,0,0,0,0,0},
      {3,5425,10367,15826,0,0,0,0,0,0,0,0,0,0,0},
      {3,6766,8245,11914,0,0,0,0,0,0,0,0,0,0,0},
      {3,8091,13882,13887,0,0,0,0,0,0,0,0,0,0,0},
      {3,1308,1348,7944,0,0,0,0,0,0,0,0,0,0,0},
      {3,4730,10272,14249,0,0,0,0,0,0,0,0,0,0,0},
      {3,5001,5838,11633,0,0,0,0,0,0,0,0,0,0,0},
      {3,3687,4732,15948,0,0,0,0,0,0,0,0,0,0,0},
      {3,285,5437,10939,0,0,0,0,0,0,0,0,0,0,0},
      {3,7254,10928,11235,0,0,0,0,0,0,0,0,0,0,0},
      {3,2038,6236,14036,0,0,0,0,0,0,0,0,0,0,0},
      {3,9407,12341,16040,0,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_140_180N[140][16]=
    {
      {15,66,862,939,3380,4920,5225,5330,6218,7204,7532,7689,9135,9363,10504,10694},
      {15,1993,2656,4602,6079,7569,7724,9038,9647,9979,11845,12641,12783,13451,13661,14166},
      {15,1360,2046,3315,3423,3974,4747,6535,6632,7261,8173,8391,9517,9928,11749,11761},
      {15,3373,3910,3965,4146,4171,6195,6384,7642,9337,9563,9716,11490,12393,13068,14016},
      {15,244,2500,3970,6097,6208,6669,7479,7667,8234,9367,10437,11623,12138,12212,12454},
      {15,665,1162,1990,2144,2356,4400,6032,9336,9988,10693,11378,12021,12582,13874,13958},
      {15,1129,1527,1725,1902,2039,2107,3241,5991,6086,7866,10793,11132,11318,13636,14100},
      {15,611,2141,2552,2602,3049,3185,5339,6495,7390,8033,9068,10844,10977,11047,13995},
      {15,2805,4137,4523,4841,7408,8551,8667,8749,8979,9232,9934,10345,10634,11646,12771},
      {15,144,1120,2295,3469,4991,5613,7186,7858,9116,9328,10850,11492,11837,12155,13103},
      {12,803,1580,1797,4719,6743,7061,7753,8376,9046,11635,11721,13350,0,0,0},
      {12,1432,3534,4822,6282,6412,7180,7619,7936,11278,12531,13074,13084,0,0,0},
      {12,2013,2575,2887,3930,4725,5498,5625,6209,6836,7268,9062,10950,0,0,0},
      {12,515,1037,2033,2624,3044,6028,7163,8729,8772,10358,10659,12466,0,0,0},
      {12,464,1685,2749,3321,3778,5322,5386,6294,7928,8871,10278,13040,0,0,0},
      {12,408,829,1672,2667,3077,3545,3703,5213,5381,7937,8474,13126,0,0,0},
      {12,1617,2490,2636,2723,5431,6975,7159,7900,10849,11572,11887,12462,0,0,0},
      {12,1402,2373,6408,6656,6704,8040,8841,9541,11818,13891,14006,14239,0,0,0},
      {12,1388,2078,2136,3514,5090,8083,8510,9200,9814,11142,11625,12980,0,0,0},
      {12,561,1659,2611,3085,3367,3804,6021,6209,6348,8282,8475,11386,0,0,0},
      {12,2457,3223,4495,4869,5314,5774,6532,6552,8987,9196,9199,11591,0,0,0},
      {12,627,1069,3015,3048,4275,4545,4617,5606,6070,8237,8659,8953,0,0,0},
      {12,1028,4096,5253,6370,8087,8382,8950,8984,9618,12843,13519,14356,0,0,0},
      {12,560,604,663,2209,2709,4421,6291,7322,10054,11747,11997,14192,0,0,0},
      {12,361,938,993,2884,3386,9431,9798,10155,11892,12184,13140,13808,0,0,0},
      {4,1045,5017,9862,13620,0,0,0,0,0,0,0,0,0,0,0},
      {4,205,3913,9136,13316,0,0,0,0,0,0,0,0,0,0,0},
      {4,2994,4307,10330,13439,0,0,0,0,0,0,0,0,0,0,0},
      {4,2437,6367,9411,10505,0,0,0,0,0,0,0,0,0,0,0},
      {4,5546,6653,7663,12391,0,0,0,0,0,0,0,0,0,0,0},
      {4,2825,3446,5803,11254,0,0,0,0,0,0,0,0,0,0,0},
      {4,1459,5988,7895,9221,0,0,0,0,0,0,0,0,0,0,0},
      {4,3968,6470,7739,12977,0,0,0,0,0,0,0,0,0,0,0},
      {4,3298,4154,12918,14282,0,0,0,0,0,0,0,0,0,0,0},
      {4,8890,9389,10144,12801,0,0,0,0,0,0,0,0,0,0,0},
      {4,2529,3668,10005,11652,0,0,0,0,0,0,0,0,0,0,0},
      {4,4558,8324,10112,12696,0,0,0,0,0,0,0,0,0,0,0},
      {4,491,6153,11815,12813,0,0,0,0,0,0,0,0,0,0,0},
      {4,1300,12716,13107,13847,0,0,0,0,0,0,0,0,0,0,0},
      {4,5542,6160,11226,12846,0,0,0,0,0,0,0,0,0,0,0},
      {3,5206,9994,11133,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7113,12088,12802,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,950,1805,8437,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4411,10474,12911,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3599,7495,8984,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4751,10097,10284,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,67,5056,11979,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10633,10770,13585,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1198,3963,9181,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,746,4895,11288,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7724,8170,9246,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6542,8235,8589,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1512,4603,11098,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7082,13053,13985,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3887,9094,10355,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3417,6588,12800,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4151,5569,8184,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5557,8162,12572,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2565,6091,6359,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2792,4430,6281,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7936,10786,11229,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,677,3184,12460,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2468,2884,11714,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,87,2318,9383,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,976,3614,10292,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,969,3180,14286,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7818,12285,13535,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3471,10797,11502,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3552,10568,12836,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1386,6971,13457,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,987,7598,9275,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5039,13533,13739,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1854,5210,11261,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10603,11753,12263,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,722,1030,12267,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,2720,5083,5350,9274,0,0,0,0,0,0,0,0,0,0,0},
      {4,3377,8717,9414,12039,0,0,0,0,0,0,0,0,0,0,0},
      {4,1355,6452,10130,13008,0,0,0,0,0,0,0,0,0,0,0},
      {4,5112,7583,9971,10955,0,0,0,0,0,0,0,0,0,0,0},
      {4,4633,8781,12791,13607,0,0,0,0,0,0,0,0,0,0,0},
      {4,1535,5803,8062,10467,0,0,0,0,0,0,0,0,0,0,0},
      {4,2326,8224,9176,12082,0,0,0,0,0,0,0,0,0,0,0},
      {4,939,8480,11823,13045,0,0,0,0,0,0,0,0,0,0,0},
      {4,380,425,4943,10388,0,0,0,0,0,0,0,0,0,0,0},
      {4,4001,4293,7887,9858,0,0,0,0,0,0,0,0,0,0,0},
      {4,3734,3746,9929,12310,0,0,0,0,0,0,0,0,0,0,0},
      {4,1592,6062,6419,10292,0,0,0,0,0,0,0,0,0,0,0},
      {4,101,2538,6316,13640,0,0,0,0,0,0,0,0,0,0,0},
      {4,3038,5921,6156,6529,0,0,0,0,0,0,0,0,0,0,0},
      {4,3820,10279,12229,12404,0,0,0,0,0,0,0,0,0,0,0},
      {3,761,3735,8874,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4985,9636,14004,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1744,2501,9257,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3223,7816,10249,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,765,2768,5363,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4911,5948,13726,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6745,9749,11658,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1373,4860,13952,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,120,407,13132,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,862,2571,3681,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3706,5914,8019,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7465,10479,12795,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,441,1017,1563,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6638,8730,10379,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3229,4169,11333,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1181,7252,11670,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1090,4576,8680,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,943,9116,11566,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3180,7882,12535,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2944,4411,12747,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3153,5618,7782,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,428,2208,10359,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,447,6906,12192,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8495,11164,12870,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,641,6397,11868,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4165,4534,11544,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4594,7957,11969,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3667,4604,7920,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2253,4617,13638,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1099,4214,6076,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,461,8085,9875,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8128,13331,13740,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8527,9749,12563,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4216,6105,12391,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6583,13362,14130,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,566,2898,4772,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4048,7696,8342,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,539,5111,9923,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,931,3789,7276,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5306,13717,13901,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1540,11240,11353,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1845,2752,6810,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8553,10094,10228,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9625,12761,13252,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4518,4526,9215,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5394,6447,10864,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7497,11962,12795,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2679,3310,3743,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2764,10853,12702,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6409,9498,10387,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_154_180N[154][14]=
    {
      {13,726,794,1587,2475,3114,3917,4471,6207,7451,8203,8218,8583,8941},
      {13,418,480,1320,1357,1481,2323,3677,5112,7038,7198,8066,9260,9282},
      {13,1506,2585,3336,4543,4828,5571,5954,6047,6081,7691,8090,8824,9153},
      {13,164,888,1867,2685,2983,4071,4848,4930,5882,7085,7861,8545,8689},
      {13,766,1004,1143,1782,1996,2506,2944,3094,4085,5728,8634,8737,8759},
      {13,199,341,2068,2100,2708,2896,4173,5846,6167,7798,9101,9159,9329},
      {13,617,752,1647,2258,2597,4708,5808,6726,7293,7800,7988,8825,9055},
      {13,315,408,620,1256,1985,2560,3226,5526,6463,6927,7223,7505,7669},
      {13,1361,1528,2397,3246,3370,4333,5167,5333,7409,8075,8197,8279,9138},
      {13,665,876,2039,2703,2864,3485,3767,4823,7275,7599,8274,8694,9334},
      {12,1160,1717,1750,2158,3040,3506,3764,3828,4261,4292,5134,6789,0},
      {12,1857,2119,2952,6145,6897,7582,7639,8032,8080,8181,8221,8454,0},
      {12,421,794,1511,2166,2489,3936,4288,5440,5901,6490,7266,8858,0},
      {12,456,2565,4071,4395,4451,4888,5338,5351,6608,7602,7835,9024,0},
      {12,617,912,3362,4085,4404,5050,5244,6043,6444,6721,7414,8353,0},
      {3,4535,7597,7853,0,0,0,0,0,0,0,0,0,0},
      {3,2632,4652,6491,0,0,0,0,0,0,0,0,0,0},
      {3,877,1378,8828,0,0,0,0,0,0,0,0,0,0},
      {3,434,3309,8607,0,0,0,0,0,0,0,0,0,0},
      {3,1075,2525,4103,0,0,0,0,0,0,0,0,0,0},
      {3,958,2220,3471,0,0,0,0,0,0,0,0,0,0},
      {3,2256,4350,7277,0,0,0,0,0,0,0,0,0,0},
      {3,1731,4306,8524,0,0,0,0,0,0,0,0,0,0},
      {3,470,6369,9026,0,0,0,0,0,0,0,0,0,0},
      {3,2605,3171,8760,0,0,0,0,0,0,0,0,0,0},
      {3,1886,4900,7558,0,0,0,0,0,0,0,0,0,0},
      {3,3825,4488,9230,0,0,0,0,0,0,0,0,0,0},
      {3,228,3806,8101,0,0,0,0,0,0,0,0,0,0},
      {3,3607,7835,8035,0,0,0,0,0,0,0,0,0,0},
      {3,5280,7413,8380,0,0,0,0,0,0,0,0,0,0},
      {3,2606,5100,5549,0,0,0,0,0,0,0,0,0,0},
      {3,2309,4329,8570,0,0,0,0,0,0,0,0,0,0},
      {3,1577,4746,7473,0,0,0,0,0,0,0,0,0,0},
      {3,2939,4664,7327,0,0,0,0,0,0,0,0,0,0},
      {3,2440,8572,8912,0,0,0,0,0,0,0,0,0,0},
      {3,4143,8221,8561,0,0,0,0,0,0,0,0,0,0},
      {3,3982,5316,7329,0,0,0,0,0,0,0,0,0,0},
      {3,387,745,5199,0,0,0,0,0,0,0,0,0,0},
      {3,2563,4077,9076,0,0,0,0,0,0,0,0,0,0},
      {3,1428,6482,9178,0,0,0,0,0,0,0,0,0,0},
      {3,4600,7640,8483,0,0,0,0,0,0,0,0,0,0},
      {3,3918,5239,5388,0,0,0,0,0,0,0,0,0,0},
      {3,2006,6768,9041,0,0,0,0,0,0,0,0,0,0},
      {3,5766,7058,7758,0,0,0,0,0,0,0,0,0,0},
      {3,2741,3382,5713,0,0,0,0,0,0,0,0,0,0},
      {3,116,1863,4193,0,0,0,0,0,0,0,0,0,0},
      {3,2339,4499,8437,0,0,0,0,0,0,0,0,0,0},
      {3,1799,5408,6711,0,0,0,0,0,0,0,0,0,0},
      {3,6937,7536,8100,0,0,0,0,0,0,0,0,0,0},
      {3,8313,8388,9277,0,0,0,0,0,0,0,0,0,0},
      {3,1743,3100,7424,0,0,0,0,0,0,0,0,0,0},
      {3,1959,2130,2230,0,0,0,0,0,0,0,0,0,0},
      {3,5007,6692,7748,0,0,0,0,0,0,0,0,0,0},
      {3,808,3333,5951,0,0,0,0,0,0,0,0,0,0},
      {3,1719,7648,8645,0,0,0,0,0,0,0,0,0,0},
      {3,102,2996,6153,0,0,0,0,0,0,0,0,0,0},
      {3,739,2765,6496,0,0,0,0,0,0,0,0,0,0},
      {3,1107,1760,7023,0,0,0,0,0,0,0,0,0,0},
      {3,1067,2666,9235,0,0,0,0,0,0,0,0,0,0},
      {3,1125,3760,8095,0,0,0,0,0,0,0,0,0,0},
      {3,2047,3143,6383,0,0,0,0,0,0,0,0,0,0},
      {3,2698,3440,5405,0,0,0,0,0,0,0,0,0,0},
      {3,1746,1854,1965,0,0,0,0,0,0,0,0,0,0},
      {3,380,3171,5816,0,0,0,0,0,0,0,0,0,0},
      {3,4155,5210,9325,0,0,0,0,0,0,0,0,0,0},
      {3,2290,2658,3766,0,0,0,0,0,0,0,0,0,0},
      {3,167,6239,6635,0,0,0,0,0,0,0,0,0,0},
      {3,1338,1541,5813,0,0,0,0,0,0,0,0,0,0},
      {3,6148,6574,7436,0,0,0,0,0,0,0,0,0,0},
      {3,3598,3777,6367,0,0,0,0,0,0,0,0,0,0},
      {3,731,4247,8887,0,0,0,0,0,0,0,0,0,0},
      {3,2152,2600,8950,0,0,0,0,0,0,0,0,0,0},
      {3,3774,4099,6605,0,0,0,0,0,0,0,0,0,0},
      {3,2819,3315,6492,0,0,0,0,0,0,0,0,0,0},
      {3,1195,3774,7126,0,0,0,0,0,0,0,0,0,0},
      {12,572,2723,3467,3509,5379,6756,6853,7335,7973,8087,8202,9000,0},
      {12,817,3136,3533,3732,5001,5148,5202,5402,6602,7236,7605,8275,0},
      {12,185,325,591,1559,1635,2826,3352,3634,3937,5814,8101,8133,0},
      {12,758,1780,1965,2932,4010,4621,7103,7249,7328,7878,8754,8805,0},
      {12,528,1433,2926,3557,3780,4650,4671,5253,5488,5517,5681,8300,0},
      {12,1172,2131,3702,4455,4504,5216,5920,6371,6660,7953,9036,9185,0},
      {12,639,1572,1714,1957,3145,5123,5330,5419,6418,7163,7237,9235,0},
      {12,166,486,708,1071,2179,3700,4756,5606,5685,6426,6467,6902,0},
      {12,462,486,735,2065,2558,3196,4006,5344,5617,7451,8141,8436,0},
      {12,435,3016,4173,4235,4415,4731,5396,7340,8002,8155,8279,9081,0},
      {5,560,2200,2649,3690,8636,0,0,0,0,0,0,0,0},
      {5,4156,5971,7133,7480,8218,0,0,0,0,0,0,0,0},
      {5,1398,2219,3796,4877,6376,0,0,0,0,0,0,0,0},
      {5,506,1284,6906,7288,9131,0,0,0,0,0,0,0,0},
      {5,643,1661,5057,8011,8241,0,0,0,0,0,0,0,0},
      {3,859,3508,5030,0,0,0,0,0,0,0,0,0,0},
      {3,575,3942,6198,0,0,0,0,0,0,0,0,0,0},
      {3,3472,5037,8710,0,0,0,0,0,0,0,0,0,0},
      {3,3850,8080,9216,0,0,0,0,0,0,0,0,0,0},
      {3,3203,8128,8836,0,0,0,0,0,0,0,0,0,0},
      {3,3059,5057,8120,0,0,0,0,0,0,0,0,0,0},
      {3,3804,6339,8539,0,0,0,0,0,0,0,0,0,0},
      {3,2355,6922,8235,0,0,0,0,0,0,0,0,0,0},
      {3,2035,2133,7090,0,0,0,0,0,0,0,0,0,0},
      {3,4787,5994,6966,0,0,0,0,0,0,0,0,0,0},
      {3,1484,4897,7766,0,0,0,0,0,0,0,0,0,0},
      {3,3977,7211,7682,0,0,0,0,0,0,0,0,0,0},
      {3,3030,4150,7823,0,0,0,0,0,0,0,0,0,0},
      {3,516,2443,7919,0,0,0,0,0,0,0,0,0,0},
      {3,5120,5715,6141,0,0,0,0,0,0,0,0,0,0},
      {3,1551,6029,7124,0,0,0,0,0,0,0,0,0,0},
      {3,1995,2156,6952,0,0,0,0,0,0,0,0,0,0},
      {3,4686,4944,8175,0,0,0,0,0,0,0,0,0,0},
      {3,2763,4667,7284,0,0,0,0,0,0,0,0,0,0},
      {3,3648,7312,7361,0,0,0,0,0,0,0,0,0,0},
      {3,333,3231,4029,0,0,0,0,0,0,0,0,0,0},
      {3,692,2273,9068,0,0,0,0,0,0,0,0,0,0},
      {3,15,3757,7203,0,0,0,0,0,0,0,0,0,0},
      {3,2870,4217,8458,0,0,0,0,0,0,0,0,0,0},
      {3,1501,2721,6548,0,0,0,0,0,0,0,0,0,0},
      {3,92,2144,6570,0,0,0,0,0,0,0,0,0,0},
      {3,1846,4578,7972,0,0,0,0,0,0,0,0,0,0},
      {3,2999,3542,4001,0,0,0,0,0,0,0,0,0,0},
      {3,1658,8479,8763,0,0,0,0,0,0,0,0,0,0},
      {3,4169,6305,7774,0,0,0,0,0,0,0,0,0,0},
      {3,2357,2475,8504,0,0,0,0,0,0,0,0,0,0},
      {3,1418,1516,3587,0,0,0,0,0,0,0,0,0,0},
      {3,2715,2754,7789,0,0,0,0,0,0,0,0,0,0},
      {3,1765,2387,8858,0,0,0,0,0,0,0,0,0,0},
      {3,5115,8712,9029,0,0,0,0,0,0,0,0,0,0},
      {3,160,2544,5818,0,0,0,0,0,0,0,0,0,0},
      {3,1600,3668,7706,0,0,0,0,0,0,0,0,0,0},
      {3,1589,3143,7396,0,0,0,0,0,0,0,0,0,0},
      {3,3310,3953,8862,0,0,0,0,0,0,0,0,0,0},
      {3,2054,3075,4821,0,0,0,0,0,0,0,0,0,0},
      {3,4061,4355,6130,0,0,0,0,0,0,0,0,0,0},
      {3,2086,2534,4831,0,0,0,0,0,0,0,0,0,0},
      {3,4229,4981,9057,0,0,0,0,0,0,0,0,0,0},
      {3,24,5398,6062,0,0,0,0,0,0,0,0,0,0},
      {3,1370,7446,8116,0,0,0,0,0,0,0,0,0,0},
      {3,409,1199,6499,0,0,0,0,0,0,0,0,0,0},
      {3,1088,1648,7267,0,0,0,0,0,0,0,0,0,0},
      {3,176,8059,9351,0,0,0,0,0,0,0,0,0,0},
      {3,558,3830,4748,0,0,0,0,0,0,0,0,0,0},
      {3,4772,8116,8277,0,0,0,0,0,0,0,0,0,0},
      {3,1253,2418,3450,0,0,0,0,0,0,0,0,0,0},
      {3,5305,5679,7537,0,0,0,0,0,0,0,0,0,0},
      {3,437,561,7932,0,0,0,0,0,0,0,0,0,0},
      {3,3058,4317,9184,0,0,0,0,0,0,0,0,0,0},
      {3,382,1516,6576,0,0,0,0,0,0,0,0,0,0},
      {3,471,6158,7469,0,0,0,0,0,0,0,0,0,0},
      {3,5,955,2716,0,0,0,0,0,0,0,0,0,0},
      {3,964,5239,8890,0,0,0,0,0,0,0,0,0,0},
      {3,727,738,4868,0,0,0,0,0,0,0,0,0,0},
      {3,7443,7560,7580,0,0,0,0,0,0,0,0,0,0},
      {3,2075,2266,8918,0,0,0,0,0,0,0,0,0,0},
      {3,4021,4267,6797,0,0,0,0,0,0,0,0,0,0},
      {3,6103,6111,8823,0,0,0,0,0,0,0,0,0,0},
      {3,6523,6531,9063,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_18_30N[108][20]=
    {
      {19,113,1557,3316,5680,6241,10407,13404,13947,14040,14353,15522,15698,16079,17363,19374,19543,20530,22833,24339},
      {19,271,1361,6236,7006,7307,7333,12768,15441,15568,17923,18341,20321,21502,22023,23938,25351,25590,25876,25910},
      {19,73,605,872,4008,6279,7653,10346,10799,12482,12935,13604,15909,16526,19782,20506,22804,23629,24859,25600},
      {19,1445,1690,4304,4851,8919,9176,9252,13783,16076,16675,17274,18806,18882,20819,21958,22451,23869,23999,24177},
      {19,1290,2337,5661,6371,8996,10102,10941,11360,12242,14918,16808,20571,23374,24046,25045,25060,25662,25783,25913},
      {19,28,42,1926,3421,3503,8558,9453,10168,15820,17473,19571,19685,22790,23336,23367,23890,24061,25657,25680},
      {19,0,1709,4041,4932,5968,7123,8430,9564,10596,11026,14761,19484,20762,20858,23803,24016,24795,25853,25863},
      {19,29,1625,6500,6609,16831,18517,18568,18738,19387,20159,20544,21603,21941,24137,24269,24416,24803,25154,25395},
      {19,55,66,871,3700,11426,13221,15001,16367,17601,18380,22796,23488,23938,25476,25635,25678,25807,25857,25872},
      {19,1,19,5958,8548,8860,11489,16845,18450,18469,19496,20190,23173,25262,25566,25668,25679,25858,25888,25915},
      {19,7520,7690,8855,9183,14654,16695,17121,17854,18083,18428,19633,20470,20736,21720,22335,23273,25083,25293,25403},
      {19,48,58,410,1299,3786,10668,18523,18963,20864,22106,22308,23033,23107,23128,23990,24286,24409,24595,25802},
      {19,12,51,3894,6539,8276,10885,11644,12777,13427,14039,15954,17078,19053,20537,22863,24521,25087,25463,25838},
      {19,3509,8748,9581,11509,15884,16230,17583,19264,20900,21001,21310,22547,22756,22959,24768,24814,25594,25626,25880},
      {19,21,29,69,1448,2386,4601,6626,6667,10242,13141,13852,14137,18640,19951,22449,23454,24431,25512,25814},
      {19,18,53,7890,9934,10063,16728,19040,19809,20825,21522,21800,23582,24556,25031,25547,25562,25733,25789,25906},
      {19,4096,4582,5766,5894,6517,10027,12182,13247,15207,17041,18958,20133,20503,22228,24332,24613,25689,25855,25883},
      {19,0,25,819,5539,7076,7536,7695,9532,13668,15051,17683,19665,20253,21996,24136,24890,25758,25784,25807},
      {19,34,40,44,4215,6076,7427,7965,8777,11017,15593,19542,22202,22973,23397,23423,24418,24873,25107,25644},
      {4,1595,6216,22850,25439,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,1562,15172,19517,22362,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,7508,12879,24324,24496,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,6298,15819,16757,18721,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,11173,15175,19966,21195,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,59,13505,16941,23793,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,2267,4830,12023,20587,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,8827,9278,13072,16664,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,14419,17463,23398,25348,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,6112,16534,20423,22698,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,493,8914,21103,24799,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,6896,12761,13206,25873,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,2,1380,12322,21701,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,11600,21306,25753,25790,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,8421,13076,14271,15401,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,9630,14112,19017,20955,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,212,13932,21781,25824,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,5961,9110,16654,19636,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,58,5434,9936,12770,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6575,11433,19798,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2731,7338,20926,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,14253,18463,25404,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,21791,24805,25869,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2,11646,15850,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6075,8586,23819,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,18435,22093,24852,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2103,2368,11704,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10925,17402,18232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9062,25061,25674,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,18497,20853,23404,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,18606,19364,19551,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7,1022,25543,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6744,15481,25868,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9081,17305,25164,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8,23701,25883,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9680,19955,22848,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,56,4564,19121,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5595,15086,25892,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3174,17127,23183,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,19397,19817,20275,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12561,24571,25825,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7111,9889,25865,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,19104,20189,21851,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,549,9686,25548,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6586,20325,25906,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3224,20710,21637,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,641,15215,25754,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,13484,23729,25818,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2043,7493,24246,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,16860,25230,25768,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,22047,24200,24902,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9391,18040,19499,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7855,24336,25069,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,23834,25570,25852,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1977,8800,25756,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6671,21772,25859,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3279,6710,24444,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,24099,25117,25820,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5553,12306,25915,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,48,11107,23907,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10832,11974,25773,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2223,17905,25484,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,16782,17135,20446,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,475,2861,3457,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,16218,22449,24362,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11716,22200,25897,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8315,15009,22633,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,13,20480,25852,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12352,18658,25687,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3681,14794,23703,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,30,24531,25846,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4103,22077,24107,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,23837,25622,25812,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3627,13387,25839,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,908,5367,19388,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,0,6894,25795,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,20322,23546,25181,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8178,25260,25437,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2449,13244,22565,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,31,18928,22741,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1312,5134,14838,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6085,13937,24220,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,66,14633,25670,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,47,22512,25472,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8867,24704,25279,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6742,21623,22745,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,147,9948,24178,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8522,24261,24307,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,19202,22406,24609,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_20_30N[120][17]=
    {
      {16,692,1779,1973,2726,5151,6088,7921,9618,11804,13043,15975,16214,16889,16980,18585,18648},
      {16,13,4090,4319,5288,8102,10110,10481,10527,10953,11185,12069,13177,14217,15963,17661,20959},
      {16,2330,2516,2902,4087,6338,8015,8638,9436,10294,10843,11802,12304,12371,14095,18486,18996},
      {16,125,586,5137,5701,6432,6500,8131,8327,10488,11032,11334,11449,12504,16000,20753,21317},
      {16,30,480,2681,3635,3898,4058,12803,14734,20252,20306,20680,21329,21333,21466,21562,21568},
      {16,20,44,738,4965,5516,7659,8464,8759,12216,14630,18241,18711,19093,20217,21316,21490},
      {16,31,43,3554,5289,5667,8687,14885,16579,17883,18384,18486,19142,20785,20932,21131,21308},
      {16,7054,9276,10435,12324,12354,13849,14285,16482,19212,19217,19221,20499,20831,20925,21195,21247},
      {16,9,13,4099,10353,10747,14884,15492,17650,19291,19394,20356,20658,21068,21117,21183,21586},
      {16,28,2250,2980,8988,10282,12503,13301,18351,20546,20622,21006,21293,21344,21472,21530,21542},
      {16,17,32,2521,4374,5098,7525,13035,14437,15283,18635,19136,20240,21147,21179,21300,21349},
      {16,57,4735,5657,7649,8807,12375,16092,16178,16379,17545,19461,19489,20321,20530,21453,21457},
      {16,35,55,5333,14423,14670,15438,19468,19667,20823,21084,21241,21344,21447,21520,21554,21586},
      {16,13,20,2025,11854,12516,14938,15929,18081,19730,19929,20408,21338,21391,21425,21468,21546},
      {16,54,7451,8176,10136,15240,16442,16482,19431,19483,19762,20647,20839,20966,21512,21579,21592},
      {16,26,465,3604,4233,9831,11741,13692,18953,18974,21021,21039,21133,21282,21488,21532,21558},
      {16,1,7,16,59,6979,7675,7717,9791,12370,13050,18534,18729,19846,19864,20127,20165},
      {16,15,31,11089,12360,13640,14237,17937,18043,18410,19443,21107,21444,21449,21528,21576,21584},
      {16,32,51,9768,17848,18095,19326,19594,19618,19765,20440,20482,20582,21236,21338,21563,21587},
      {16,44,55,4864,10253,11306,12117,13076,13901,15610,17057,18205,19794,20939,21132,21267,21573},
      {16,3436,11304,15361,16511,16860,18238,18639,19341,20106,20123,20407,21200,21280,21452,21526,21569},
      {16,679,8822,11045,14403,16588,17838,19117,19453,20265,20558,21374,21396,21428,21442,21529,21590},
      {16,391,13002,13140,14314,17169,17175,17846,18122,19447,20075,20212,20436,20583,21330,21359,21403},
      {4,7601,10257,20060,21285,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,4419,9150,18097,20315,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4675,13376,21435,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,610,1238,16704,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5732,7096,21104,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5690,13531,14545,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4334,14839,17357,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8,2814,17674,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2392,8128,18369,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,502,7403,15133,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,343,13624,20673,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,13188,15687,21593,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,321,16866,21347,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1242,4261,17449,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4691,8086,8691,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8500,11538,20278,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6269,12905,18192,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5984,15452,17111,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11541,18717,21534,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,16,10780,16107,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12310,12959,20390,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1365,18306,19634,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6125,19132,20242,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3012,17233,21533,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5816,13021,21440,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,13207,17811,18798,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2762,7586,12139,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3949,5545,13584,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11374,18279,19241,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2736,10989,21209,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4095,20677,21395,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8251,10084,20498,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7628,8875,21406,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2743,8943,9090,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1817,7788,15767,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9333,9838,21268,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6203,9480,12042,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5747,21187,21468,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2553,18281,21500,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3179,9155,15222,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12498,18109,20326,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,14106,21209,21592,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7454,17484,20791,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,20804,21120,21574,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5754,18178,20935,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,30,4322,21381,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11905,20416,21397,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12452,19899,21497,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1917,6028,16868,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9891,18710,18953,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,912,21083,21446,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,370,14355,18069,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,16519,19003,20902,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11163,17558,18424,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8427,14396,21405,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8885,11796,21361,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4960,15431,20653,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11944,16839,21236,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9967,14529,17208,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,14144,19354,19745,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7986,12680,21396,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6097,11501,13028,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,33,13803,21038,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3177,20124,20803,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2692,6841,18655,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,971,5892,14354,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3887,19455,21271,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,17214,17315,21148,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6539,13910,21526,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3809,5153,15793,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3865,21438,21510,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7129,17787,19636,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5972,13150,14182,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7078,14906,16911,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,15705,21160,21482,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5479,13860,19763,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,16817,19722,20001,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,14649,16147,18886,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,15138,18578,21502,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2096,2534,17760,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11920,13460,19783,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,19876,20071,20583,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6241,14230,20775,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,16138,16386,21371,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8616,15624,18453,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6013,8015,21599,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9184,10688,20792,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,18122,21141,21469,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10706,13177,20957,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,15148,15584,20959,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9114,9432,16467,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5483,14687,14705,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8325,21161,21410,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2328,17670,19834,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7015,20802,21385,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,52,5451,20379,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9689,15537,19733,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_22_30N[132][16]=
    {
      {15,696,989,1238,3091,3116,3738,4269,6406,7033,8048,9157,10254,12033,16456,16912},
      {15,444,1488,6541,8626,10735,12447,13111,13706,14135,15195,15947,16453,16916,17137,17268},
      {15,401,460,992,1145,1576,1678,2238,2320,4280,6770,10027,12486,15363,16714,17157},
      {15,1161,3108,3727,4508,5092,5348,5582,7727,11793,12515,12917,13362,14247,16717,17205},
      {15,542,1190,6883,7911,8349,8835,10489,11631,14195,15009,15454,15482,16632,17040,17063},
      {15,17,487,776,880,5077,6172,9771,11446,12798,16016,16109,16171,17087,17132,17226},
      {15,1337,3275,3462,4229,9246,10180,10845,10866,12250,13633,14482,16024,16812,17186,17241},
      {15,15,980,2305,3674,5971,8224,11499,11752,11770,12897,14082,14836,15311,16391,17209},
      {15,0,3926,5869,8696,9351,9391,11371,14052,14172,14636,14974,16619,16961,17033,17237},
      {15,3033,5317,6501,8579,10698,12168,12966,14019,15392,15806,15991,16493,16690,17062,17090},
      {15,981,1205,4400,6410,11003,13319,13405,14695,15846,16297,16492,16563,16616,16862,16953},
      {15,1725,4276,8869,9588,14062,14486,15474,15548,16300,16432,17042,17050,17060,17175,17273},
      {15,1807,5921,9960,10011,14305,14490,14872,15852,16054,16061,16306,16799,16833,17136,17262},
      {15,2826,4752,6017,6540,7016,8201,14245,14419,14716,15983,16569,16652,17171,17179,17247},
      {15,1662,2516,3345,5229,8086,9686,11456,12210,14595,15808,16011,16421,16825,17112,17195},
      {15,2890,4821,5987,7226,8823,9869,12468,14694,15352,15805,16075,16462,17102,17251,17263},
      {15,3751,3890,4382,5720,10281,10411,11350,12721,13121,14127,14980,15202,15335,16735,17123},
      {15,26,30,2805,5457,6630,7188,7477,7556,11065,16608,16859,16909,16943,17030,17103},
      {15,40,4524,5043,5566,9645,10204,10282,11696,13080,14837,15607,16274,17034,17225,17266},
      {15,904,3157,6284,7151,7984,11712,12887,13767,15547,16099,16753,16829,17044,17250,17259},
      {15,7,311,4876,8334,9249,11267,14072,14559,15003,15235,15686,16331,17177,17238,17253},
      {15,4410,8066,8596,9631,10369,11249,12610,15769,16791,16960,17018,17037,17062,17165,17204},
      {15,24,8261,9691,10138,11607,12782,12786,13424,13933,15262,15795,16476,17084,17193,17220},
      {4,88,11622,14705,15890,0,0,0,0,0,0,0,0,0,0,0},
      {4,304,2026,2638,6018,0,0,0,0,0,0,0,0,0,0,0},
      {4,1163,4268,11620,17232,0,0,0,0,0,0,0,0,0,0,0},
      {4,9701,11785,14463,17260,0,0,0,0,0,0,0,0,0,0,0},
      {4,4118,10952,12224,17006,0,0,0,0,0,0,0,0,0,0,0},
      {4,3647,10823,11521,12060,0,0,0,0,0,0,0,0,0,0,0},
      {4,1717,3753,9199,11642,0,0,0,0,0,0,0,0,0,0,0},
      {3,2187,14280,17220,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,14787,16903,17061,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,381,3534,4294,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3149,6947,8323,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12562,16724,16881,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7289,9997,15306,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5615,13152,17260,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5666,16926,17027,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4190,7798,16831,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4778,10629,17180,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10001,13884,15453,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6,2237,8203,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7831,15144,15160,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9186,17204,17243,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9435,17168,17237,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,42,5701,17159,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,7812,14259,15715,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,39,4513,6658,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,38,9368,11273,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1119,4785,17182,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5620,16521,16729,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,16,6685,17242,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,210,3452,12383,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,466,14462,16250,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10548,12633,13962,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1452,6005,16453,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,22,4120,13684,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5195,11563,16522,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5518,16705,17201,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12233,14552,15471,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6067,13440,17248,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8660,8967,17061,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8673,12176,15051,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5959,15767,16541,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3244,12109,12414,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,31,15913,16323,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3270,15686,16653,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,24,7346,14675,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12,1531,8740,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6228,7565,16667,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,16936,17122,17162,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4868,8451,13183,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3714,4451,16919,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11313,13801,17132,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,17070,17191,17242,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1911,11201,17186,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,14,17190,17254,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11760,16008,16832,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,14543,17033,17278,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,16129,16765,17155,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6891,15561,17007,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,12741,14744,17116,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8992,16661,17277,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1861,11130,16742,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4822,13331,16192,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,13281,14027,14989,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,38,14887,17141,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10698,13452,15674,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4,2539,16877,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,857,17170,17249,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11449,11906,12867,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,285,14118,16831,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,15191,17214,17242,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,39,728,16915,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2469,12969,15579,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,16644,17151,17164,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2592,8280,10448,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9236,12431,17173,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9064,16892,17233,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4526,16146,17038,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,31,2116,16083,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,15837,16951,17031,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5362,8382,16618,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6137,13199,17221,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2841,15068,17068,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,24,3620,17003,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9880,15718,16764,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1784,10240,17209,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2731,10293,10846,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3121,8723,16598,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8563,15662,17088,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,13,1167,14676,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,29,13850,15963,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3654,7553,8114,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,23,4362,14865,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4434,14741,16688,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8362,13901,17244,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,13687,16736,17232,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,46,4229,13394,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,13169,16383,16972,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,16031,16681,16952,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3384,9894,12580,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,9841,14414,16165,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5013,17099,17115,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2130,8941,17266,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,6907,15428,17241,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,16,1860,17235,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2151,16014,16643,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,14954,15958,17222,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3969,8419,15116,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,31,15593,16984,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11514,16605,17255,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_1_4S[9][13]=
    {
      {12,6295,9626,304,7695,4839,4936,1660,144,11203,5567,6347,12557},
      {12,10691,4988,3859,3734,3071,3494,7687,10313,5964,8069,8296,11090},
      {12,10774,3613,5208,11177,7676,3549,8746,6583,7239,12265,2674,4292},
      {12,11869,3708,5981,8718,4908,10650,6805,3334,2627,10461,9285,11120},
      {3,7844,3079,10773,0,0,0,0,0,0,0,0,0},
      {3,3385,10854,5747,0,0,0,0,0,0,0,0,0},
      {3,1360,12010,12202,0,0,0,0,0,0,0,0,0},
      {3,6189,4241,2343,0,0,0,0,0,0,0,0,0},
      {3,9840,12726,4977,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_1_3S[15][13]=
    {
      {12,416,8909,4156,3216,3112,2560,2912,6405,8593,4969,6723,6912},
      {12,8978,3011,4339,9312,6396,2957,7288,5485,6031,10218,2226,3575},
      {12,3383,10059,1114,10008,10147,9384,4290,434,5139,3536,1965,2291},
      {12,2797,3693,7615,7077,743,1941,8716,6215,3840,5140,4582,5420},
      {12,6110,8551,1515,7404,4879,4946,5383,1831,3441,9569,10472,4306},
      {3,1505,5682,7778,0,0,0,0,0,0,0,0,0},
      {3,7172,6830,6623,0,0,0,0,0,0,0,0,0},
      {3,7281,3941,3505,0,0,0,0,0,0,0,0,0},
      {3,10270,8669,914,0,0,0,0,0,0,0,0,0},
      {3,3622,7563,9388,0,0,0,0,0,0,0,0,0},
      {3,9930,5058,4554,0,0,0,0,0,0,0,0,0},
      {3,4844,9609,2707,0,0,0,0,0,0,0,0,0},
      {3,6883,3237,1714,0,0,0,0,0,0,0,0,0},
      {3,4768,3878,10017,0,0,0,0,0,0,0,0,0},
      {3,10127,3334,8267,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_2_5S[18][13]=
    {
      {12,5650,4143,8750,583,6720,8071,635,1767,1344,6922,738,6658},
      {12,5696,1685,3207,415,7019,5023,5608,2605,857,6915,1770,8016},
      {12,3992,771,2190,7258,8970,7792,1802,1866,6137,8841,886,1931},
      {12,4108,3781,7577,6810,9322,8226,5396,5867,4428,8827,7766,2254},
      {12,4247,888,4367,8821,9660,324,5864,4774,227,7889,6405,8963},
      {12,9693,500,2520,2227,1811,9330,1928,5140,4030,4824,806,3134},
      {3,1652,8171,1435,0,0,0,0,0,0,0,0,0},
      {3,3366,6543,3745,0,0,0,0,0,0,0,0,0},
      {3,9286,8509,4645,0,0,0,0,0,0,0,0,0},
      {3,7397,5790,8972,0,0,0,0,0,0,0,0,0},
      {3,6597,4422,1799,0,0,0,0,0,0,0,0,0},
      {3,9276,4041,3847,0,0,0,0,0,0,0,0,0},
      {3,8683,7378,4946,0,0,0,0,0,0,0,0,0},
      {3,5348,1993,9186,0,0,0,0,0,0,0,0,0},
      {3,6724,9015,5646,0,0,0,0,0,0,0,0,0},
      {3,4502,4439,8474,0,0,0,0,0,0,0,0,0},
      {3,5107,7342,9442,0,0,0,0,0,0,0,0,0},
      {3,1387,8910,2660,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_1_2S[20][9]=
    {
      {8,20,712,2386,6354,4061,1062,5045,5158},
      {8,21,2543,5748,4822,2348,3089,6328,5876},
      {8,22,926,5701,269,3693,2438,3190,3507},
      {8,23,2802,4520,3577,5324,1091,4667,4449},
      {8,24,5140,2003,1263,4742,6497,1185,6202},
      {3,0,4046,6934,0,0,0,0,0},
      {3,1,2855,66,0,0,0,0,0},
      {3,2,6694,212,0,0,0,0,0},
      {3,3,3439,1158,0,0,0,0,0},
      {3,4,3850,4422,0,0,0,0,0},
      {3,5,5924,290,0,0,0,0,0},
      {3,6,1467,4049,0,0,0,0,0},
      {3,7,7820,2242,0,0,0,0,0},
      {3,8,4606,3080,0,0,0,0,0},
      {3,9,4633,7877,0,0,0,0,0},
      {3,10,3884,6868,0,0,0,0,0},
      {3,11,8935,4996,0,0,0,0,0},
      {3,12,3028,764,0,0,0,0,0},
      {3,13,5988,1057,0,0,0,0,0},
      {3,14,7411,3450,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_3_5S_DVBT2[27][13]=
    {
      {12,71,1478,1901,2240,2649,2725,3592,3708,3965,4080,5733,6198},
      {12,393,1384,1435,1878,2773,3182,3586,5465,6091,6110,6114,6327},
      {12,160,1149,1281,1526,1566,2129,2929,3095,3223,4250,4276,4612},
      {12,289,1446,1602,2421,3559,3796,5590,5750,5763,6168,6271,6340},
      {12,947,1227,2008,2020,2266,3365,3588,3867,4172,4250,4865,6290},
      {3,3324,3704,4447,0,0,0,0,0,0,0,0,0},
      {3,1206,2565,3089,0,0,0,0,0,0,0,0,0},
      {3,529,4027,5891,0,0,0,0,0,0,0,0,0},
      {3,141,1187,3206,0,0,0,0,0,0,0,0,0},
      {3,1990,2972,5120,0,0,0,0,0,0,0,0,0},
      {3,752,796,5976,0,0,0,0,0,0,0,0,0},
      {3,1129,2377,4030,0,0,0,0,0,0,0,0,0},
      {3,6077,6108,6231,0,0,0,0,0,0,0,0,0},
      {3,61,1053,1781,0,0,0,0,0,0,0,0,0},
      {3,2820,4109,5307,0,0,0,0,0,0,0,0,0},
      {3,2088,5834,5988,0,0,0,0,0,0,0,0,0},
      {3,3725,3945,4010,0,0,0,0,0,0,0,0,0},
      {3,1081,2780,3389,0,0,0,0,0,0,0,0,0},
      {3,659,2221,4822,0,0,0,0,0,0,0,0,0},
      {3,3033,6060,6160,0,0,0,0,0,0,0,0,0},
      {3,756,1489,2350,0,0,0,0,0,0,0,0,0},
      {3,3350,3624,5470,0,0,0,0,0,0,0,0,0},
      {3,357,1825,5242,0,0,0,0,0,0,0,0,0},
      {3,585,3372,6062,0,0,0,0,0,0,0,0,0},
      {3,561,1417,2348,0,0,0,0,0,0,0,0,0},
      {3,971,3719,5567,0,0,0,0,0,0,0,0,0},
      {3,1005,1675,2062,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_3_5S_DVBS2[27][13]=
    {
      {12,2765,5713,6426,3596,1374,4811,2182,544,3394,2840,4310,771},
      {12,4951,211,2208,723,1246,2928,398,5739,265,5601,5993,2615},
      {12,210,4730,5777,3096,4282,6238,4939,1119,6463,5298,6320,4016},
      {12,4167,2063,4757,3157,5664,3956,6045,563,4284,2441,3412,6334},
      {12,4201,2428,4474,59,1721,736,2997,428,3807,1513,4732,6195},
      {12,2670,3081,5139,3736,1999,5889,4362,3806,4534,5409,6384,5809},
      {12,5516,1622,2906,3285,1257,5797,3816,817,875,2311,3543,1205},
      {12,4244,2184,5415,1705,5642,4886,2333,287,1848,1121,3595,6022},
      {12,2142,2830,4069,5654,1295,2951,3919,1356,884,1786,396,4738},
      {3,0,2161,2653,0,0,0,0,0,0,0,0,0},
      {3,1,1380,1461,0,0,0,0,0,0,0,0,0},
      {3,2,2502,3707,0,0,0,0,0,0,0,0,0},
      {3,3,3971,1057,0,0,0,0,0,0,0,0,0},
      {3,4,5985,6062,0,0,0,0,0,0,0,0,0},
      {3,5,1733,6028,0,0,0,0,0,0,0,0,0},
      {3,6,3786,1936,0,0,0,0,0,0,0,0,0},
      {3,7,4292,956,0,0,0,0,0,0,0,0,0},
      {3,8,5692,3417,0,0,0,0,0,0,0,0,0},
      {3,9,266,4878,0,0,0,0,0,0,0,0,0},
      {3,10,4913,3247,0,0,0,0,0,0,0,0,0},
      {3,11,4763,3937,0,0,0,0,0,0,0,0,0},
      {3,12,3590,2903,0,0,0,0,0,0,0,0,0},
      {3,13,2566,4215,0,0,0,0,0,0,0,0,0},
      {3,14,5208,4707,0,0,0,0,0,0,0,0,0},
      {3,15,3940,3388,0,0,0,0,0,0,0,0,0},
      {3,16,5109,4556,0,0,0,0,0,0,0,0,0},
      {3,17,4908,4177,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_2_3S[30][14]=
    {
      {13,0,2084,1613,1548,1286,1460,3196,4297,2481,3369,3451,4620,2622},
      {13,1,122,1516,3448,2880,1407,1847,3799,3529,373,971,4358,3108},
      {13,2,259,3399,929,2650,864,3996,3833,107,5287,164,3125,2350},
      {3,3,342,3529,0,0,0,0,0,0,0,0,0,0},
      {3,4,4198,2147,0,0,0,0,0,0,0,0,0,0},
      {3,5,1880,4836,0,0,0,0,0,0,0,0,0,0},
      {3,6,3864,4910,0,0,0,0,0,0,0,0,0,0},
      {3,7,243,1542,0,0,0,0,0,0,0,0,0,0},
      {3,8,3011,1436,0,0,0,0,0,0,0,0,0,0},
      {3,9,2167,2512,0,0,0,0,0,0,0,0,0,0},
      {3,10,4606,1003,0,0,0,0,0,0,0,0,0,0},
      {3,11,2835,705,0,0,0,0,0,0,0,0,0,0},
      {3,12,3426,2365,0,0,0,0,0,0,0,0,0,0},
      {3,13,3848,2474,0,0,0,0,0,0,0,0,0,0},
      {3,14,1360,1743,0,0,0,0,0,0,0,0,0,0},
      {3,0,163,2536,0,0,0,0,0,0,0,0,0,0},
      {3,1,2583,1180,0,0,0,0,0,0,0,0,0,0},
      {3,2,1542,509,0,0,0,0,0,0,0,0,0,0},
      {3,3,4418,1005,0,0,0,0,0,0,0,0,0,0},
      {3,4,5212,5117,0,0,0,0,0,0,0,0,0,0},
      {3,5,2155,2922,0,0,0,0,0,0,0,0,0,0},
      {3,6,347,2696,0,0,0,0,0,0,0,0,0,0},
      {3,7,226,4296,0,0,0,0,0,0,0,0,0,0},
      {3,8,1560,487,0,0,0,0,0,0,0,0,0,0},
      {3,9,3926,1640,0,0,0,0,0,0,0,0,0,0},
      {3,10,149,2928,0,0,0,0,0,0,0,0,0,0},
      {3,11,2364,563,0,0,0,0,0,0,0,0,0,0},
      {3,12,635,688,0,0,0,0,0,0,0,0,0,0},
      {3,13,231,1684,0,0,0,0,0,0,0,0,0,0},
      {3,14,1129,3894,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_3_4S[33][13]=
    {
      {12,3,3198,478,4207,1481,1009,2616,1924,3437,554,683,1801},
      {3,4,2681,2135,0,0,0,0,0,0,0,0,0},
      {3,5,3107,4027,0,0,0,0,0,0,0,0,0},
      {3,6,2637,3373,0,0,0,0,0,0,0,0,0},
      {3,7,3830,3449,0,0,0,0,0,0,0,0,0},
      {3,8,4129,2060,0,0,0,0,0,0,0,0,0},
      {3,9,4184,2742,0,0,0,0,0,0,0,0,0},
      {3,10,3946,1070,0,0,0,0,0,0,0,0,0},
      {3,11,2239,984,0,0,0,0,0,0,0,0,0},
      {3,0,1458,3031,0,0,0,0,0,0,0,0,0},
      {3,1,3003,1328,0,0,0,0,0,0,0,0,0},
      {3,2,1137,1716,0,0,0,0,0,0,0,0,0},
      {3,3,132,3725,0,0,0,0,0,0,0,0,0},
      {3,4,1817,638,0,0,0,0,0,0,0,0,0},
      {3,5,1774,3447,0,0,0,0,0,0,0,0,0},
      {3,6,3632,1257,0,0,0,0,0,0,0,0,0},
      {3,7,542,3694,0,0,0,0,0,0,0,0,0},
      {3,8,1015,1945,0,0,0,0,0,0,0,0,0},
      {3,9,1948,412,0,0,0,0,0,0,0,0,0},
      {3,10,995,2238,0,0,0,0,0,0,0,0,0},
      {3,11,4141,1907,0,0,0,0,0,0,0,0,0},
      {3,0,2480,3079,0,0,0,0,0,0,0,0,0},
      {3,1,3021,1088,0,0,0,0,0,0,0,0,0},
      {3,2,713,1379,0,0,0,0,0,0,0,0,0},
      {3,3,997,3903,0,0,0,0,0,0,0,0,0},
      {3,4,2323,3361,0,0,0,0,0,0,0,0,0},
      {3,5,1110,986,0,0,0,0,0,0,0,0,0},
      {3,6,2532,142,0,0,0,0,0,0,0,0,0},
      {3,7,1690,2405,0,0,0,0,0,0,0,0,0},
      {3,8,1298,1881,0,0,0,0,0,0,0,0,0},
      {3,9,615,174,0,0,0,0,0,0,0,0,0},
      {3,10,1648,3112,0,0,0,0,0,0,0,0,0},
      {3,11,1415,2808,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_4_5S[35][4]=
    {
      {3,5,896,1565},
      {3,6,2493,184},
      {3,7,212,3210},
      {3,8,727,1339},
      {3,9,3428,612},
      {3,0,2663,1947},
      {3,1,230,2695},
      {3,2,2025,2794},
      {3,3,3039,283},
      {3,4,862,2889},
      {3,5,376,2110},
      {3,6,2034,2286},
      {3,7,951,2068},
      {3,8,3108,3542},
      {3,9,307,1421},
      {3,0,2272,1197},
      {3,1,1800,3280},
      {3,2,331,2308},
      {3,3,465,2552},
      {3,4,1038,2479},
      {3,5,1383,343},
      {3,6,94,236},
      {3,7,2619,121},
      {3,8,1497,2774},
      {3,9,2116,1855},
      {3,0,722,1584},
      {3,1,2767,1881},
      {3,2,2701,1610},
      {3,3,3283,1732},
      {3,4,168,1099},
      {3,5,3074,243},
      {3,6,3460,945},
      {3,7,2049,1746},
      {3,8,566,1427},
      {3,9,3545,1168}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_5_6S[37][14]=
    {
      {13,3,2409,499,1481,908,559,716,1270,333,2508,2264,1702,2805},
      {3,4,2447,1926,0,0,0,0,0,0,0,0,0,0},
      {3,5,414,1224,0,0,0,0,0,0,0,0,0,0},
      {3,6,2114,842,0,0,0,0,0,0,0,0,0,0},
      {3,7,212,573,0,0,0,0,0,0,0,0,0,0},
      {3,0,2383,2112,0,0,0,0,0,0,0,0,0,0},
      {3,1,2286,2348,0,0,0,0,0,0,0,0,0,0},
      {3,2,545,819,0,0,0,0,0,0,0,0,0,0},
      {3,3,1264,143,0,0,0,0,0,0,0,0,0,0},
      {3,4,1701,2258,0,0,0,0,0,0,0,0,0,0},
      {3,5,964,166,0,0,0,0,0,0,0,0,0,0},
      {3,6,114,2413,0,0,0,0,0,0,0,0,0,0},
      {3,7,2243,81,0,0,0,0,0,0,0,0,0,0},
      {3,0,1245,1581,0,0,0,0,0,0,0,0,0,0},
      {3,1,775,169,0,0,0,0,0,0,0,0,0,0},
      {3,2,1696,1104,0,0,0,0,0,0,0,0,0,0},
      {3,3,1914,2831,0,0,0,0,0,0,0,0,0,0},
      {3,4,532,1450,0,0,0,0,0,0,0,0,0,0},
      {3,5,91,974,0,0,0,0,0,0,0,0,0,0},
      {3,6,497,2228,0,0,0,0,0,0,0,0,0,0},
      {3,7,2326,1579,0,0,0,0,0,0,0,0,0,0},
      {3,0,2482,256,0,0,0,0,0,0,0,0,0,0},
      {3,1,1117,1261,0,0,0,0,0,0,0,0,0,0},
      {3,2,1257,1658,0,0,0,0,0,0,0,0,0,0},
      {3,3,1478,1225,0,0,0,0,0,0,0,0,0,0},
      {3,4,2511,980,0,0,0,0,0,0,0,0,0,0},
      {3,5,2320,2675,0,0,0,0,0,0,0,0,0,0},
      {3,6,435,1278,0,0,0,0,0,0,0,0,0,0},
      {3,7,228,503,0,0,0,0,0,0,0,0,0,0},
      {3,0,1885,2369,0,0,0,0,0,0,0,0,0,0},
      {3,1,57,483,0,0,0,0,0,0,0,0,0,0},
      {3,2,838,1050,0,0,0,0,0,0,0,0,0,0},
      {3,3,1231,1990,0,0,0,0,0,0,0,0,0,0},
      {3,4,1738,68,0,0,0,0,0,0,0,0,0,0},
      {3,5,2392,951,0,0,0,0,0,0,0,0,0,0},
      {3,6,163,645,0,0,0,0,0,0,0,0,0,0},
      {3,7,2644,1704,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_8_9S[40][5]=
    {
      {4,0,1558,712,805},
      {4,1,1450,873,1337},
      {4,2,1741,1129,1184},
      {4,3,294,806,1566},
      {4,4,482,605,923},
      {3,0,926,1578,0},
      {3,1,777,1374,0},
      {3,2,608,151,0},
      {3,3,1195,210,0},
      {3,4,1484,692,0},
      {3,0,427,488,0},
      {3,1,828,1124,0},
      {3,2,874,1366,0},
      {3,3,1500,835,0},
      {3,4,1496,502,0},
      {3,0,1006,1701,0},
      {3,1,1155,97,0},
      {3,2,657,1403,0},
      {3,3,1453,624,0},
      {3,4,429,1495,0},
      {3,0,809,385,0},
      {3,1,367,151,0},
      {3,2,1323,202,0},
      {3,3,960,318,0},
      {3,4,1451,1039,0},
      {3,0,1098,1722,0},
      {3,1,1015,1428,0},
      {3,2,1261,1564,0},
      {3,3,544,1190,0},
      {3,4,1472,1246,0},
      {3,0,508,630,0},
      {3,1,421,1704,0},
      {3,2,284,898,0},
      {3,3,392,577,0},
      {3,4,1155,556,0},
      {3,0,631,1000,0},
      {3,1,732,1368,0},
      {3,2,1328,329,0},
      {3,3,1515,506,0},
      {3,4,1104,1172,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_11_45S[11][11]=
    {
      {10,9054,9186,12155,1000,7383,6459,2992,4723,8135,11250},
      {10,2624,9237,7139,12238,11962,4361,5292,10967,11036,8105},
      {10,2044,11996,5654,7568,7002,3549,4767,8767,2872,8345},
      {10,6966,8473,5180,8084,3359,5051,9576,5139,1893,902},
      {10,3041,3801,8252,11951,909,8535,1038,8400,3200,4585},
      {3,5291,10484,10872,0,0,0,0,0,0,0},
      {3,442,7516,3720,0,0,0,0,0,0,0},
      {3,11469,769,10998,0,0,0,0,0,0,0},
      {3,10575,1436,2935,0,0,0,0,0,0,0},
      {3,6905,8610,11285,0,0,0,0,0,0,0},
      {3,1873,5634,6383,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_4_15S[12][22]=
    {
      {21,1953,2331,2545,2623,4653,5012,5700,6458,6875,7605,7694,7881,8416,8758,9181,9555,9578,9932,10068,11479,11699},
      {21,514,784,2059,2129,2386,2454,3396,5184,6624,6825,7533,7861,9116,9473,9601,10432,11011,11159,11378,11528,11598},
      {21,483,1303,1735,2291,3302,3648,4222,4522,5511,6626,6804,7404,7752,7982,8108,8930,9151,9793,9876,10786,11879},
      {4,1956,7572,9020,9971,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,13,1578,7445,8373,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,6805,6857,8615,11179,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,7983,8022,10017,11748,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,4939,8861,10444,11661,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,2278,3733,6265,10009,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,4494,7974,10649,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,8909,11030,11696,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3131,9964,10480,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_14_45S[14][13]=
    {
      {12,1606,3617,7973,6737,9495,4209,9209,4565,4250,7823,9384,400},
      {12,4105,991,923,3562,3892,10993,5640,8196,6652,4653,9116,7677},
      {12,6348,1341,5445,1494,7799,831,4952,5106,3011,9921,6537,8476},
      {12,7854,5274,8572,3741,5674,11128,4097,1398,5671,7302,8155,2641},
      {12,6548,2103,590,5749,5722,10,2682,1063,633,2949,207,6065},
      {9,2828,6366,4766,399,935,7611,84,150,3146,0,0,0},
      {3,5363,7455,7140,0,0,0,0,0,0,0,0,0},
      {3,9297,482,4848,0,0,0,0,0,0,0,0,0},
      {3,8458,1631,5344,0,0,0,0,0,0,0,0,0},
      {3,5729,6767,4836,0,0,0,0,0,0,0,0,0},
      {3,11019,4463,3882,0,0,0,0,0,0,0,0,0},
      {3,4107,9610,5454,0,0,0,0,0,0,0,0,0},
      {3,11137,4328,6307,0,0,0,0,0,0,0,0,0},
      {3,3260,7897,3809,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_7_15S[21][25]=
    {
      {24,3,137,314,327,983,1597,2028,3043,3217,4109,6020,6178,6535,6560,7146,7180,7408,7790,7893,8123,8313,8526,8616,8638},
      {24,356,1197,1208,1839,1903,2712,3088,3537,4091,4301,4919,5068,6025,6195,6324,6378,6686,6829,7558,7745,8042,8382,8587,8602},
      {24,18,187,1115,1417,1463,2300,2328,3502,3805,4677,4827,5551,5968,6394,6412,6753,7169,7524,7695,7976,8069,8118,8522,8582},
      {24,714,2713,2726,2964,3055,3220,3334,3459,5557,5765,5841,6290,6419,6573,6856,7786,7937,8156,8286,8327,8384,8448,8539,8559},
      {4,3452,7935,8092,8623,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,56,1955,3000,8242,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,1809,4094,7991,8489,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,2220,6455,7849,8548,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,1006,2576,3247,6976,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,2177,6048,7795,8295,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,1413,2595,7446,8594,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,2101,3714,7541,8531,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,10,5961,7484,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3144,4636,5282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5708,5875,8390,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3322,5223,7975,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,197,4653,8283,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,598,5393,8624,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,906,7249,7542,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1223,2148,8195,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,976,2001,5005,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_8_15S[24][22]=
    {
      {21,32,384,430,591,1296,1976,1999,2137,2175,3638,4214,4304,4486,4662,4999,5174,5700,6969,7115,7138,7189},
      {21,1788,1881,1910,2724,4504,4928,4973,5616,5686,5718,5846,6523,6893,6994,7074,7100,7277,7399,7476,7480,7537},
      {21,2791,2824,2927,4196,4298,4800,4948,5361,5401,5688,5818,5862,5969,6029,6244,6645,6962,7203,7302,7454,7534},
      {21,574,1461,1826,2056,2069,2387,2794,3349,3366,4951,5826,5834,5903,6640,6762,6786,6859,7043,7418,7431,7554},
      {21,14,178,675,823,890,930,1209,1311,2898,4339,4600,5203,6485,6549,6970,7208,7218,7298,7454,7457,7462},
      {4,4075,4188,7313,7553,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,5145,6018,7148,7507,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,3198,4858,6983,7033,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,3170,5126,5625,6901,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {4,2839,6093,7071,7450,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,11,3735,5413,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2497,5400,7238,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2067,5172,5714,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1889,7173,7329,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1795,2773,3499,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2695,2944,6735,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,3221,4625,5897,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1690,6122,6816,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,5013,6839,7358,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1601,6849,7415,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2180,7389,7543,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,2121,6838,7054,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,1948,3109,5046,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {3,272,1015,7464,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_26_45S[26][14]=
    {
      {13,6106,5389,698,6749,6294,1653,1984,2167,6139,6095,3832,2468,6115},
      {13,4202,2362,1852,1264,3564,6345,498,6137,3908,3302,527,2767,6667},
      {12,3422,1242,1377,2238,2899,1974,1957,261,3463,4994,215,2338,0},
      {12,3016,5109,6533,2665,5300,4908,4967,5787,726,229,1970,2789,0},
      {12,6146,5765,6649,2871,884,1670,2597,5058,3659,6594,5042,304,0},
      {12,5521,2811,0,4214,2626,2211,1236,3771,852,6356,6797,3463,0},
      {12,1523,1830,3938,5593,2128,5791,3421,3680,6692,1377,3808,3475,0},
      {12,5551,6035,2247,3662,759,6783,116,6380,4586,3367,1,5003,0},
      {3,3518,6557,6510,0,0,0,0,0,0,0,0,0,0},
      {3,1830,839,4421,0,0,0,0,0,0,0,0,0,0},
      {3,5431,5959,6152,0,0,0,0,0,0,0,0,0,0},
      {3,3174,5113,4520,0,0,0,0,0,0,0,0,0,0},
      {3,5399,1303,2496,0,0,0,0,0,0,0,0,0,0},
      {3,2841,741,220,0,0,0,0,0,0,0,0,0,0},
      {3,2731,1830,4193,0,0,0,0,0,0,0,0,0,0},
      {3,1875,3935,223,0,0,0,0,0,0,0,0,0,0},
      {3,9,4720,423,0,0,0,0,0,0,0,0,0,0},
      {3,3107,2676,840,0,0,0,0,0,0,0,0,0,0},
      {3,1950,6177,6457,0,0,0,0,0,0,0,0,0,0},
      {3,4091,94,5102,0,0,0,0,0,0,0,0,0,0},
      {3,1907,6050,3455,0,0,0,0,0,0,0,0,0,0},
      {3,714,3,559,0,0,0,0,0,0,0,0,0,0},
      {3,502,4268,4164,0,0,0,0,0,0,0,0,0,0},
      {3,1019,5558,271,0,0,0,0,0,0,0,0,0,0},
      {3,6127,854,3221,0,0,0,0,0,0,0,0,0,0},
      {3,959,5337,2735,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_32_45S[32][13]=
    {
      {12,2686,655,2308,1603,336,1743,2778,1263,3555,185,4212,621},
      {12,286,2994,2599,2265,126,314,3992,4560,2845,2764,2540,1476},
      {12,2670,3599,2900,2281,3597,2768,4423,2805,836,130,1204,4162},
      {12,1884,4228,1253,2578,3053,3650,2587,4468,2784,1644,1490,4655},
      {12,4258,1699,4363,4555,3810,4046,3806,344,2459,4067,3327,3510},
      {5,1021,2741,2528,2168,2820,0,0,0,0,0,0,0},
      {3,254,1080,616,0,0,0,0,0,0,0,0,0},
      {3,1465,4192,2972,0,0,0,0,0,0,0,0,0},
      {3,2356,2976,1534,0,0,0,0,0,0,0,0,0},
      {3,4412,1937,2724,0,0,0,0,0,0,0,0,0},
      {3,1430,3024,600,0,0,0,0,0,0,0,0,0},
      {3,1952,2136,3573,0,0,0,0,0,0,0,0,0},
      {3,3009,3123,1288,0,0,0,0,0,0,0,0,0},
      {3,4553,2299,806,0,0,0,0,0,0,0,0,0},
      {3,2997,402,4330,0,0,0,0,0,0,0,0,0},
      {3,3302,4567,698,0,0,0,0,0,0,0,0,0},
      {3,2364,498,3146,0,0,0,0,0,0,0,0,0},
      {3,1809,647,992,0,0,0,0,0,0,0,0,0},
      {3,3512,32,4301,0,0,0,0,0,0,0,0,0},
      {3,1238,251,450,0,0,0,0,0,0,0,0,0},
      {3,1657,737,641,0,0,0,0,0,0,0,0,0},
      {3,560,1720,2893,0,0,0,0,0,0,0,0,0},
      {3,1689,2206,902,0,0,0,0,0,0,0,0,0},
      {3,3998,1784,2094,0,0,0,0,0,0,0,0,0},
      {3,2090,3126,1201,0,0,0,0,0,0,0,0,0},
      {3,1565,764,3473,0,0,0,0,0,0,0,0,0},
      {3,891,903,2413,0,0,0,0,0,0,0,0,0},
      {3,2286,2900,2348,0,0,0,0,0,0,0,0,0},
      {3,3026,2033,1502,0,0,0,0,0,0,0,0,0},
      {3,2404,1243,556,0,0,0,0,0,0,0,0,0},
      {3,308,2222,3825,0,0,0,0,0,0,0,0,0},
      {3,1523,3311,389,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_1_5M[18][14]=
    {
      {13,18222,6715,4908,21568,22821,11708,4769,4495,22243,25872,9051,19072,13956},
      {13,2038,5205,21215,21009,9584,2403,23652,20866,20130,677,9509,6136,773},
      {13,19936,14590,17829,473,4432,23171,11386,17937,22084,24450,267,8822,19335},
      {13,16376,16769,5111,9794,18907,827,12385,12370,21647,10938,23619,11633,15865},
      {13,23417,7631,12243,21546,4192,22117,14757,4118,9686,17021,8531,15989,8807},
      {13,15533,16584,18529,19699,17821,4252,1254,5952,3163,20295,6944,1022,19743},
      {13,129,16579,23524,25897,14690,11222,16250,9925,4268,999,7102,24528,152},
      {13,18361,3708,3454,16604,1551,5809,20324,4775,22418,19091,19674,10975,7327},
      {13,24133,10950,22779,11388,13818,20668,7556,12333,16446,19684,12510,25118,8162},
      {3,17026,6850,1269,0,0,0,0,0,0,0,0,0,0},
      {3,21895,7137,25270,0,0,0,0,0,0,0,0,0,0},
      {3,11858,24153,13303,0,0,0,0,0,0,0,0,0,0},
      {3,7885,16438,12805,0,0,0,0,0,0,0,0,0,0},
      {3,10473,15004,8052,0,0,0,0,0,0,0,0,0,0},
      {3,2088,10379,10067,0,0,0,0,0,0,0,0,0,0},
      {3,21438,13426,10440,0,0,0,0,0,0,0,0,0,0},
      {3,17696,727,12164,0,0,0,0,0,0,0,0,0,0},
      {3,22623,8408,17849,0,0,0,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_11_45M[22][11]=
    {
      {10,20617,6867,14845,11974,22563,190,17207,4052,7406,16007},
      {10,21448,14846,2543,23380,16633,20365,16869,13411,19853,795},
      {10,5200,2330,2775,23620,20643,10745,14742,6493,14222,20939},
      {10,9445,9523,12769,7332,21792,18717,16397,14016,9481,22162},
      {10,2922,6427,4497,4116,17658,2581,14364,3781,18851,22974},
      {10,10383,2184,1433,3889,12828,17424,17580,20936,1390,21374},
      {10,425,2063,22398,20907,9445,14790,4457,723,7048,4072},
      {10,11771,9640,23212,9613,12042,8335,21386,20129,13521,16301},
      {10,14867,12501,1086,21526,17701,17731,20907,8790,19224,5784},
      {10,7107,19690,17616,5800,9501,23320,16878,794,15931,17539},
      {3,4556,21783,1524,0,0,0,0,0,0,0},
      {3,20100,11706,23663,0,0,0,0,0,0,0},
      {3,2535,15530,6116,0,0,0,0,0,0,0},
      {3,12078,3867,2663,0,0,0,0,0,0,0},
      {3,19629,20246,7024,0,0,0,0,0,0,0},
      {3,11748,11426,19802,0,0,0,0,0,0,0},
      {3,15942,12333,5316,0,0,0,0,0,0,0},
      {3,11521,3170,17818,0,0,0,0,0,0,0},
      {3,2289,23780,16575,0,0,0,0,0,0,0},
      {3,6649,16991,13025,0,0,0,0,0,0,0},
      {3,20050,10619,10250,0,0,0,0,0,0,0},
      {3,3944,13063,5656,0,0,0,0,0,0,0}
    };

    const int dvb_bch_ldpc_bb_impl::ldpc_tab_1_3M[30][13]=
    {
      {12,7416,4093,16722,1023,20586,12219,9175,16284,1554,10113,19849,17545},
      {12,13140,3257,2110,13888,3023,1537,1598,15018,18931,13905,10617,1014},
      {12,339,14366,3309,15360,18358,3196,4412,6023,7070,17380,2777,6691},
      {12,12720,17634,4141,1400,8247,18201,16077,11314,11928,3494,3155,2865},
      {12,21038,6928,3860,1943,20292,6526,12939,15182,3957,5651,356,2673},
      {12,20555,17905,5724,13932,1218,17763,5912,5164,6233,6220,1277,19209},
      {12,19190,4498,4950,6645,5482,5667,13701,16334,15231,735,8589,12344},
      {12,679,17849,17807,16033,10181,3368,5778,8275,2736,14042,17506,6005},
      {12,1576,10259,10525,3007,16522,697,7726,8641,14323,2893,8168,11070},
      {12,17270,10180,18740,847,4969,14715,19316,5530,17428,11193,9861,13562},
      {3,6156,18787,10467,0,0,0,0,0,0,0,0,0},
      {3,2422,3723,10769,0,0,0,0,0,0,0,0,0},
      {3,8015,18716,13406,0,0,0,0,0,0,0,0,0},
      {3,5969,15949,3084,0,0,0,0,0,0,0,0,0},
      {3,6855,13990,3764,0,0,0,0,0,0,0,0,0},
      {3,10351,15779,10392,0,0,0,0,0,0,0,0,0},
      {3,16078,19034,11279,0,0,0,0,0,0,0,0,0},
      {3,11747,6608,4188,0,0,0,0,0,0,0,0,0},
      {3,19699,8928,8045,0,0,0,0,0,0,0,0,0},
      {3,4598,7219,11391,0,0,0,0,0,0,0,0,0},
      {3,19766,11871,5692,0,0,0,0,0,0,0,0,0},
      {3,7487,15905,17621,0,0,0,0,0,0,0,0,0},
      {3,8554,7751,16516,0,0,0,0,0,0,0,0,0},
      {3,4981,20250,16146,0,0,0,0,0,0,0,0,0},
      {3,12524,21364,10793,0,0,0,0,0,0,0,0,0},
      {3,17083,2051,8862,0,0,0,0,0,0,0,0,0},
      {3,1315,6246,7721,0,0,0,0,0,0,0,0,0},
      {3,18043,16652,5502,0,0,0,0,0,0,0,0,0},
      {3,1432,5674,2224,0,0,0,0,0,0,0,0,0},
      {3,11257,1312,8453,0,0,0,0,0,0,0,0,0}
    };


    void
    dvb_bch_ldpc_bb_impl::bch_poly_build_tables(void)
    {
      // Normal polynomials
      const int polyn01[]={1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,1};
      const int polyn02[]={1,1,0,0,1,1,1,0,1,0,0,0,0,0,0,0,1};
      const int polyn03[]={1,0,1,1,1,1,0,1,1,1,1,1,0,0,0,0,1};
      const int polyn04[]={1,0,1,0,1,0,1,0,0,1,0,1,1,0,1,0,1};
      const int polyn05[]={1,1,1,1,0,1,0,0,1,1,1,1,1,0,0,0,1};
      const int polyn06[]={1,0,1,0,1,1,0,1,1,1,1,0,1,1,1,1,1};
      const int polyn07[]={1,0,1,0,0,1,1,0,1,1,1,1,0,1,0,1,1};
      const int polyn08[]={1,1,1,0,0,1,1,0,1,1,0,0,1,1,1,0,1};
      const int polyn09[]={1,0,0,0,0,1,0,1,0,1,1,1,0,0,0,0,1};
      const int polyn10[]={1,1,1,0,0,1,0,1,1,0,1,0,1,1,1,0,1};
      const int polyn11[]={1,0,1,1,0,1,0,0,0,1,0,1,1,1,0,0,1};
      const int polyn12[]={1,1,0,0,0,1,1,1,0,1,0,1,1,0,0,0,1};

      // Medium polynomials
      const int polym01[]={1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,1};
      const int polym02[]={1,1,0,0,1,0,0,1,0,0,1,1,0,0,0,1};
      const int polym03[]={1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,1};
      const int polym04[]={1,0,1,1,0,1,1,0,1,0,1,1,0,0,0,1};
      const int polym05[]={1,1,1,0,1,0,1,1,0,0,1,0,1,0,0,1};
      const int polym06[]={1,0,0,0,1,0,1,1,0,0,0,0,1,1,0,1};
      const int polym07[]={1,0,1,0,1,1,0,1,0,0,0,1,1,0,1,1};
      const int polym08[]={1,0,1,0,1,0,1,0,1,1,0,1,0,0,1,1};
      const int polym09[]={1,1,1,0,1,1,0,1,0,1,0,1,1,1,0,1};
      const int polym10[]={1,1,1,1,1,0,0,1,0,0,1,1,1,1,0,1};
      const int polym11[]={1,1,1,0,1,0,0,0,0,1,0,1,0,0,0,1};
      const int polym12[]={1,0,1,0,1,0,0,0,1,0,1,1,0,1,1,1};

      // Short polynomials
      const int polys01[]={1,1,0,1,0,1,0,0,0,0,0,0,0,0,1};
      const int polys02[]={1,0,0,0,0,0,1,0,1,0,0,1,0,0,1};
      const int polys03[]={1,1,1,0,0,0,1,0,0,1,1,0,0,0,1};
      const int polys04[]={1,0,0,0,1,0,0,1,1,0,1,0,1,0,1};
      const int polys05[]={1,0,1,0,1,0,1,0,1,1,0,1,0,1,1};
      const int polys06[]={1,0,0,1,0,0,0,1,1,1,0,0,0,1,1};
      const int polys07[]={1,0,1,0,0,1,1,1,0,0,1,1,0,1,1};
      const int polys08[]={1,0,0,0,0,1,0,0,1,1,1,1,0,0,1};
      const int polys09[]={1,1,1,1,0,0,0,0,0,1,1,0,0,0,1};
      const int polys10[]={1,0,0,1,0,0,1,0,0,1,0,1,1,0,1};
      const int polys11[]={1,0,0,0,1,0,0,0,0,0,0,1,1,0,1};
      const int polys12[]={1,1,1,1,0,1,1,1,1,0,1,0,0,1,1};

      int len;
      int polyout[2][200];

      len = poly_mult(polyn01, 17, polyn02,    17,  polyout[0]);
      len = poly_mult(polyn03, 17, polyout[0], len, polyout[1]);
      len = poly_mult(polyn04, 17, polyout[1], len, polyout[0]);
      len = poly_mult(polyn05, 17, polyout[0], len, polyout[1]);
      len = poly_mult(polyn06, 17, polyout[1], len, polyout[0]);
      len = poly_mult(polyn07, 17, polyout[0], len, polyout[1]);
      len = poly_mult(polyn08, 17, polyout[1], len, polyout[0]);
      poly_pack(polyout[0], m_poly_n_8, 128);

      len = poly_mult(polyn09, 17, polyout[0], len, polyout[1]);
      len = poly_mult(polyn10, 17, polyout[1], len, polyout[0]);
      poly_pack(polyout[0], m_poly_n_10, 160);

      len = poly_mult(polyn11, 17, polyout[0], len, polyout[1]);
      len = poly_mult(polyn12, 17, polyout[1], len, polyout[0]);
      poly_pack(polyout[0], m_poly_n_12, 192);

      len = poly_mult(polys01, 15, polys02,    15,  polyout[0]);
      len = poly_mult(polys03, 15, polyout[0], len, polyout[1]);
      len = poly_mult(polys04, 15, polyout[1], len, polyout[0]);
      len = poly_mult(polys05, 15, polyout[0], len, polyout[1]);
      len = poly_mult(polys06, 15, polyout[1], len, polyout[0]);
      len = poly_mult(polys07, 15, polyout[0], len, polyout[1]);
      len = poly_mult(polys08, 15, polyout[1], len, polyout[0]);
      len = poly_mult(polys09, 15, polyout[0], len, polyout[1]);
      len = poly_mult(polys10, 15, polyout[1], len, polyout[0]);
      len = poly_mult(polys11, 15, polyout[0], len, polyout[1]);
      len = poly_mult(polys12, 15, polyout[1], len, polyout[0]);
      poly_pack(polyout[0], m_poly_s_12, 168);

      len = poly_mult(polym01, 16, polym02,    16,  polyout[0]);
      len = poly_mult(polym03, 16, polyout[0], len, polyout[1]);
      len = poly_mult(polym04, 16, polyout[1], len, polyout[0]);
      len = poly_mult(polym05, 16, polyout[0], len, polyout[1]);
      len = poly_mult(polym06, 16, polyout[1], len, polyout[0]);
      len = poly_mult(polym07, 16, polyout[0], len, polyout[1]);
      len = poly_mult(polym08, 16, polyout[1], len, polyout[0]);
      len = poly_mult(polym09, 16, polyout[0], len, polyout[1]);
      len = poly_mult(polym10, 16, polyout[1], len, polyout[0]);
      len = poly_mult(polym11, 16, polyout[0], len, polyout[1]);
      len = poly_mult(polym12, 16, polyout[1], len, polyout[0]);
      poly_pack(polyout[0], m_poly_m_12, 180);
    }





  } /* namespace dtv */
} /* namespace gr */
