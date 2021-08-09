/*-------------------------------------------------------------------
 *          Example algorithms f1, f1*, f2, f3, f4, f5, f5*
 *-------------------------------------------------------------------
 *
 *  A sample implementation of the example 3GPP authentication and
 *  key agreement functions f1, f1*, f2, f3, f4, f5 and f5*.  This is
 *  a byte-oriented implementation of the functions, and of the block
 *  cipher kernel function Rijndael.
 *
 *  This has been coded for clarity, not necessarily for efficiency.
 *
 *  The functions f2, f3, f4 and f5 share the same inputs and have
 *  been coded together as a single function.  f1, f1* and f5* are
 *  all coded separately.
 *
 *-----------------------------------------------------------------*/

/************************ 3gpp 35.206 *****************************/
#include "authentication_algorithms_with_5gaka.hpp"
typedef unsigned char u8;

/*--------- Operator Variant Algorithm Configuration Field --------*/

/*------- Insert your value of OP here -------*/
u8 OP[16] = {0x63, 0xbf, 0xa5, 0x0e, 0xe6, 0x52, 0x33, 0x65,
             0xff, 0x14, 0xc1, 0xf4, 0x5f, 0x88, 0x73, 0x7d};
/*------- Insert your value of OP here -------*/

// u8 op_c[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
// 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
uint8_t op_c[16] = {0xcd, 0x63, 0xcb, 0x71, 0x95, 0x4a, 0x9f, 0x4e,
                    0x48, 0xa5, 0x99, 0x4e, 0x37, 0xa0, 0x2b, 0xaf};
/*--------------------------- prototypes --------------------------*/

void f1(u8 k[16], u8 rand[16], u8 sqn[6], u8 amf[2], u8 mac_a[8]);
void f2345(u8 k[16], u8 rand[16], u8 res[8], u8 ck[16], u8 ik[16], u8 ak[6]);
void f1star(u8 k[16], u8 rand[16], u8 sqn[6], u8 amf[2], u8 mac_s[8]);
void f5star(u8 k[16], u8 rand[16], u8 ak[6]);

/*-------------------------------------------------------------------
 *                            Algorithm f1
 *-------------------------------------------------------------------
 *
 *  Computes network authentication code MAC-A from key K, random
 *  challenge RAND, sequence number SQN and authentication management
 *  field AMF.
 *
 *-----------------------------------------------------------------*/

void f1(u8 k[16], u8 rand[16], u8 sqn[6], u8 amf[2], u8 mac_a[8]) {
  printf("opc: ");
  for (int i = 0; i < 16; i++) printf("0x%x", op_c[i]);
  printf("\n");
  // u8 op_c[16];
  u8 temp[16];
  u8 in1[16];
  u8 out1[16];
  u8 rijndaelInput[16];
  u8 i;

  Authentication_5gaka::RijndaelKeySchedule(k);

  // ComputeOPc( op_c );

  for (i = 0; i < 16; i++) rijndaelInput[i] = rand[i] ^ op_c[i];
  Authentication_5gaka::RijndaelEncrypt(rijndaelInput, temp);

  for (i = 0; i < 6; i++) {
    in1[i]     = sqn[i];
    in1[i + 8] = sqn[i];
  }
  for (i = 0; i < 2; i++) {
    in1[i + 6]  = amf[i];
    in1[i + 14] = amf[i];
  }

  /* XOR op_c and in1, rotate by r1=64, and XOR *
   * on the constant c1 (which is all zeroes)   */

  for (i = 0; i < 16; i++) rijndaelInput[(i + 8) % 16] = in1[i] ^ op_c[i];

  /* XOR on the value temp computed before */

  for (i = 0; i < 16; i++) rijndaelInput[i] ^= temp[i];

  Authentication_5gaka::RijndaelEncrypt(rijndaelInput, out1);
  for (i = 0; i < 16; i++) out1[i] ^= op_c[i];

  for (i = 0; i < 8; i++) mac_a[i] = out1[i];

  return;
} /* end of function f1 */

/*-------------------------------------------------------------------
 *                            Algorithms f2-f5
 *-------------------------------------------------------------------
 *
 *  Takes key K and random challenge RAND, and returns response RES,
 *  confidentiality key CK, integrity key IK and anonymity key AK.
 *
 *-----------------------------------------------------------------*/

void f2345(u8 k[16], u8 rand[16], u8 res[8], u8 ck[16], u8 ik[16], u8 ak[6]) {
  // u8 op_c[16];
  u8 temp[16];
  u8 out[16];
  u8 rijndaelInput[16];
  u8 i;

  Authentication_5gaka::RijndaelKeySchedule(k);

  // ComputeOPc( op_c );

  for (i = 0; i < 16; i++) rijndaelInput[i] = rand[i] ^ op_c[i];
  Authentication_5gaka::RijndaelEncrypt(rijndaelInput, temp);

  /* To obtain output block OUT2: XOR OPc and TEMP,    *
   * rotate by r2=0, and XOR on the constant c2 (which *
   * is all zeroes except that the last bit is 1).     */

  for (i = 0; i < 16; i++) rijndaelInput[i] = temp[i] ^ op_c[i];
  rijndaelInput[15] ^= 1;

  Authentication_5gaka::RijndaelEncrypt(rijndaelInput, out);
  for (i = 0; i < 16; i++) out[i] ^= op_c[i];

  for (i = 0; i < 8; i++) res[i] = out[i + 8];
  for (i = 0; i < 6; i++) ak[i] = out[i];

  /* To obtain output block OUT3: XOR OPc and TEMP,        *
   * rotate by r3=32, and XOR on the constant c3 (which    *
   * is all zeroes except that the next to last bit is 1). */

  for (i = 0; i < 16; i++) rijndaelInput[(i + 12) % 16] = temp[i] ^ op_c[i];
  rijndaelInput[15] ^= 2;

  Authentication_5gaka::RijndaelEncrypt(rijndaelInput, out);
  for (i = 0; i < 16; i++) out[i] ^= op_c[i];

  for (i = 0; i < 16; i++) ck[i] = out[i];

  /* To obtain output block OUT4: XOR OPc and TEMP,         *
   * rotate by r4=64, and XOR on the constant c4 (which     *
   * is all zeroes except that the 2nd from last bit is 1). */

  for (i = 0; i < 16; i++) rijndaelInput[(i + 8) % 16] = temp[i] ^ op_c[i];
  rijndaelInput[15] ^= 4;

  Authentication_5gaka::RijndaelEncrypt(rijndaelInput, out);
  for (i = 0; i < 16; i++) out[i] ^= op_c[i];

  for (i = 0; i < 16; i++) ik[i] = out[i];

  return;
} /* end of function f2345 */

/*-------------------------------------------------------------------
 *                            Algorithm f1*
 *-------------------------------------------------------------------
 *
 *  Computes resynch authentication code MAC-S from key K, random
 *  challenge RAND, sequence number SQN and authentication management
 *  field AMF.
 *
 *-----------------------------------------------------------------*/

void f1star(u8 k[16], u8 rand[16], u8 sqn[6], u8 amf[2], u8 mac_s[8]) {
  // u8 op_c[16];
  u8 temp[16];
  u8 in1[16];
  u8 out1[16];
  u8 rijndaelInput[16];
  u8 i;

  Authentication_5gaka::RijndaelKeySchedule(k);

  // ComputeOPc( op_c );

  for (i = 0; i < 16; i++) rijndaelInput[i] = rand[i] ^ op_c[i];
  Authentication_5gaka::RijndaelEncrypt(rijndaelInput, temp);

  for (i = 0; i < 6; i++) {
    in1[i]     = sqn[i];
    in1[i + 8] = sqn[i];
  }
  for (i = 0; i < 2; i++) {
    in1[i + 6]  = amf[i];
    in1[i + 14] = amf[i];
  }

  /* XOR op_c and in1, rotate by r1=64, and XOR *
   * on the constant c1 (which is all zeroes)   */

  for (i = 0; i < 16; i++) rijndaelInput[(i + 8) % 16] = in1[i] ^ op_c[i];

  /* XOR on the value temp computed before */

  for (i = 0; i < 16; i++) rijndaelInput[i] ^= temp[i];

  Authentication_5gaka::RijndaelEncrypt(rijndaelInput, out1);
  for (i = 0; i < 16; i++) out1[i] ^= op_c[i];

  for (i = 0; i < 8; i++) mac_s[i] = out1[i + 8];

  return;
} /* end of function f1star */

/*-------------------------------------------------------------------
 *                            Algorithm f5*
 *-------------------------------------------------------------------
 *
 *  Takes key K and random challenge RAND, and returns resynch
 *  anonymity key AK.
 *
 *-----------------------------------------------------------------*/

void f5star(u8 k[16], u8 rand[16], u8 ak[6]) {
  // u8 op_c[16];
  u8 temp[16];
  u8 out[16];
  u8 rijndaelInput[16];
  u8 i;

  Authentication_5gaka::RijndaelKeySchedule(k);

  // ComputeOPc( op_c );

  for (i = 0; i < 16; i++) rijndaelInput[i] = rand[i] ^ op_c[i];
  Authentication_5gaka::RijndaelEncrypt(rijndaelInput, temp);

  /* To obtain output block OUT5: XOR OPc and TEMP,         *
   * rotate by r5=96, and XOR on the constant c5 (which     *
   * is all zeroes except that the 3rd from last bit is 1). */

  for (i = 0; i < 16; i++) rijndaelInput[(i + 4) % 16] = temp[i] ^ op_c[i];
  rijndaelInput[15] ^= 8;

  Authentication_5gaka::RijndaelEncrypt(rijndaelInput, out);
  for (i = 0; i < 16; i++) out[i] ^= op_c[i];

  for (i = 0; i < 6; i++) ak[i] = out[i];

  return;
} /* end of function f5star */
