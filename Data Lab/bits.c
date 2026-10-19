/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * 学生须知：
 *
 * 步骤1：请仔细阅读以下说明。
 */

你将通过编辑此源文件中的函数集合来提交 Data Lab 的解决方案。

整数编码规则：

  将每个函数中的 "return" 语句替换为一行或多行实现该函数的 C 代码。
  你的代码必须遵循以下风格：

  int Funct(arg1, arg2, ...) {
      /* 简要描述你的实现原理 */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  每个 "Expr"（表达式）只能使用以下内容：
  1. 0 到 255（0xFF）之间的整数常量（含端点）。不允许使用大常量
     如 0xffffffff。
  2. 函数参数和局部变量（不允许使用全局变量）。
  3. 一元整数运算 ! ~
  4. 二元整数运算 & ^ | + << >>

  某些题目会进一步限制允许使用的运算符集合。
  每个 "Expr" 可以包含多个运算符，不限制每行只能使用一个运算符。

  严禁以下行为：
  1. 使用任何控制结构，如 if、do、while、for、switch 等。
  2. 定义或使用任何宏。
  3. 在此文件中定义任何额外的函数。
  4. 调用任何函数。
  5. 使用任何其他运算，如 &&、||、- 或 ?:。
  6. 使用任何形式的类型转换。
  7. 使用除 int 以外的任何数据类型。这意味着你不能使用数组、
     结构体或联合体。


  你可以假设你的机器具有以下特性：
  1. 使用 32 位二进制补码表示整数。
  2. 右移操作采用算术移位。
  3. 当移位量小于 0 或大于 31 时，移位操作的行为不可预测。


可接受的编码风格示例：
  /*
   * pow2plus1 - 返回 2^x + 1，其中 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* 利用移位运算计算 2 的幂次 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - 返回 2^x + 4，其中 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* 利用移位运算计算 2 的幂次 */
     int result = (1 << x);
     result += 4;
     return result;
  }

浮点数编码规则

对于需要你实现浮点运算的题目，编码规则相对宽松。
你可以使用循环和条件控制。你可以同时使用 int 和 unsigned 类型。
你可以使用任意的整数和 unsigned 常量。你可以对 int 或 unsigned 数据
使用任何算术、逻辑或比较运算。

严禁以下行为：
  1. 定义或使用任何宏。
  2. 在此文件中定义任何额外的函数。
  3. 调用任何函数。
  4. 使用任何形式的类型转换。
  5. 使用除 int 或 unsigned 以外的任何数据类型。这意味着你不能使用
     数组、结构体或联合体。
  6. 使用任何浮点数据类型、运算或常量。


注意事项：
  1. 使用 dlc（data lab checker）编译器（在讲义中有说明）来检查
     你的解决方案的合法性。
  2. 每个函数都有一个允许用于实现的最大操作数（整数、逻辑或比较运算）
     限制。最大操作数由 dlc 进行检查。
     注意：赋值操作（'='）不计入操作数；你可以随意使用，不受惩罚。
  3. 使用 btest 测试框架来检查你的函数是否正确。
  4. 使用 BDD 检查器来形式化验证你的函数。
  5. 每个函数的最大操作数在该函数的头部注释中给出。如果讲义中
     的最大操作数与本文档中的存在任何不一致，请以本文档为准。

/*
 * 步骤2：根据编码规则修改以下函数。
 *
 *   重要提示，为避免评分时出现意外：
 *   1. 使用 dlc 编译器检查你的解决方案是否符合编码规则。
 *   2. 使用 BDD 检查器形式化验证你的解决方案是否产生正确的结果。
 */


#endif
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
    return ~(~(x & ~y) & ~(~x & y));
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {

  return (1 << 31);

}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  return (!((x + 1u) + (x + 1u))) & (!!(x + 1u));
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  int mask = 0x55;
  mask = mask | (mask << 8);
  mask = mask | (mask << 16);
  mask = mask << 1;
  return !((x & mask) ^ mask);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return (~x) + 1;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  return !((x >> 4) ^ 0x3) & !(((x & 0xF) + 6) >> 4);
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  int sub_1 = (~1) + 1;
  int mux = (!!x) + sub_1;
  return (y & ~mux) | (z & mux);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  int neg_x = ~x + 1;
  return(!(x ^ (1<<31))) | (!(((y + neg_x) >> 31) & 1));
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  int negx = ~x + 1;
  int sign = (x >> 31) | (negx >> 31);
  return ~sign & 1;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  int mask = x >> 31;
  int y = x ^ mask;
  int bits = 0,tmp;

  tmp = !!(y >> 16) << 4;
  bits += tmp;
  y >>= tmp;

  tmp = !!(y >> 8) << 3;
  bits += tmp;
  y >>= tmp;

  tmp = !!(y >> 4) << 2;
  bits += tmp;
  y >>= tmp;

  tmp = !!(y >> 2) << 1;
  bits += tmp;
  y >>= tmp;

  tmp = !!(y >> 1);
  bits += tmp;
  y >>=tmp;

  return bits + 1 + !!y;
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
  unsigned s = uf >> 31;
  unsigned e = (uf & ((0xff) << 23)) >> 23;
  unsigned f = uf & 0x007fffff;
  if(e == 0xff){
    if(f) return uf;
    return (s << 31) | (e << 23);
  }
  if(e)
  {
    if(e == 0xfe)return (s << 31) | (0xff << 23);
    return uf + 0x800000;
  }
  return (s << 31) | (f << 1);
  
} //浮点数运算，注意nan，无穷，规格化，考虑正负
  //步骤：对阶，尾数相加，规格化，舍入，判断溢出
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  unsigned s = uf & 0x80000000;
  unsigned e = (uf & 0x7f800000) >> 23;
  unsigned m = uf & 0x7fffff;
  int exp = 0,ans = 0;
  if(e == 0xff){
    return 0x80000000u;
  }
  if(!e){
    return 0;
  }
  else{
    m = m + 0x800000;
    exp = e - 150;
    if(exp > 7)return 0x80000000u;
    if(exp >= 0){
      if(!s) return m << exp;
      return (~(m << exp)) + 1;
    }
    exp = -exp;
    if(exp > 24)return 0;
  }
  ans = (m >> exp);
  if(!s) return ans;
  return ~ans + 1;
  
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
  if(x > 127) return 0x7f800000u;
  if(x < -126) return 0u;
  return (x + 127) << 23;
}
