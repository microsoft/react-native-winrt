#pragma once

#define EXPAND_SEQUENCE1(x, y, d) x##1##y
#define EXPAND_SEQUENCE2(x, y, d) EXPAND_SEQUENCE1(x, y, d)##d x##2##y
#define EXPAND_SEQUENCE3(x, y, d) EXPAND_SEQUENCE2(x, y, d)##d x##3##y
#define EXPAND_SEQUENCE4(x, y, d) EXPAND_SEQUENCE3(x, y, d)##d x##4##y
#define EXPAND_SEQUENCE5(x, y, d) EXPAND_SEQUENCE4(x, y, d)##d x##5##y
#define EXPAND_SEQUENCE6(x, y, d) EXPAND_SEQUENCE5(x, y, d)##d x##6##y
#define EXPAND_SEQUENCE7(x, y, d) EXPAND_SEQUENCE6(x, y, d)##d x##7##y
#define EXPAND_SEQUENCE8(x, y, d) EXPAND_SEQUENCE7(x, y, d)##d x##8##y
#define EXPAND_SEQUENCE9(x, y, d) EXPAND_SEQUENCE8(x, y, d)##d x##9##y
#define EXPAND_SEQUENCE10(x, y, d) EXPAND_SEQUENCE9(x, y, d)##d x##10##y
#define EXPAND_SEQUENCE11(x, y, d) EXPAND_SEQUENCE10(x, y, d)##d x##11##y
#define EXPAND_SEQUENCE12(x, y, d) EXPAND_SEQUENCE11(x, y, d)##d x##12##y
#define EXPAND_SEQUENCE13(x, y, d) EXPAND_SEQUENCE12(x, y, d)##d x##13##y
#define EXPAND_SEQUENCE14(x, y, d) EXPAND_SEQUENCE13(x, y, d)##d x##14##y
#define EXPAND_SEQUENCE15(x, y, d) EXPAND_SEQUENCE14(x, y, d)##d x##15##y
#define EXPAND_SEQUENCE16(x, y, d) EXPAND_SEQUENCE15(x, y, d)##d x##16##y
#define EXPAND_SEQUENCE17(x, y, d) EXPAND_SEQUENCE16(x, y, d)##d x##17##y
#define EXPAND_SEQUENCE18(x, y, d) EXPAND_SEQUENCE17(x, y, d)##d x##18##y
#define EXPAND_SEQUENCE19(x, y, d) EXPAND_SEQUENCE18(x, y, d)##d x##19##y
#define EXPAND_SEQUENCE20(x, y, d) EXPAND_SEQUENCE19(x, y, d)##d x##20##y
#define EXPAND_SEQUENCE21(x, y, d) EXPAND_SEQUENCE20(x, y, d)##d x##21##y
#define EXPAND_SEQUENCE22(x, y, d) EXPAND_SEQUENCE21(x, y, d)##d x##22##y
#define EXPAND_SEQUENCE23(x, y, d) EXPAND_SEQUENCE22(x, y, d)##d x##23##y
#define EXPAND_SEQUENCE24(x, y, d) EXPAND_SEQUENCE23(x, y, d)##d x##24##y
#define EXPAND_SEQUENCE25(x, y, d) EXPAND_SEQUENCE24(x, y, d)##d x##25##y
#define EXPAND_SEQUENCE26(x, y, d) EXPAND_SEQUENCE25(x, y, d)##d x##26##y
#define EXPAND_SEQUENCE27(x, y, d) EXPAND_SEQUENCE26(x, y, d)##d x##27##y
#define EXPAND_SEQUENCE28(x, y, d) EXPAND_SEQUENCE27(x, y, d)##d x##28##y
#define EXPAND_SEQUENCE29(x, y, d) EXPAND_SEQUENCE27(x, y, d)##d x##29##y
#define EXPAND_SEQUENCE30(x, y, d) EXPAND_SEQUENCE29(x, y, d)##d x##30##y
#define EXPAND_SEQUENCE(x, y, i, d) EXPAND_SEQUENCE##i(x, y, d)

#define EXPAND_SEQUENCE_PAIR1(x, y, z, d) x##1##y##1##z
#define EXPAND_SEQUENCE_PAIR2(x, y, z, d) EXPAND_SEQUENCE_PAIR1(x, y, z, d)##d x##2##y##2##z
#define EXPAND_SEQUENCE_PAIR3(x, y, z, d) EXPAND_SEQUENCE_PAIR2(x, y, z, d)##d x##3##y##3##z
#define EXPAND_SEQUENCE_PAIR4(x, y, z, d) EXPAND_SEQUENCE_PAIR3(x, y, z, d)##d x##4##y##4##z
#define EXPAND_SEQUENCE_PAIR5(x, y, z, d) EXPAND_SEQUENCE_PAIR4(x, y, z, d)##d x##5##y##5##z
#define EXPAND_SEQUENCE_PAIR6(x, y, z, d) EXPAND_SEQUENCE_PAIR5(x, y, z, d)##d x##6##y##6##z
#define EXPAND_SEQUENCE_PAIR7(x, y, z, d) EXPAND_SEQUENCE_PAIR6(x, y, z, d)##d x##7##y##7##z
#define EXPAND_SEQUENCE_PAIR8(x, y, z, d) EXPAND_SEQUENCE_PAIR7(x, y, z, d)##d x##8##y##8##z
#define EXPAND_SEQUENCE_PAIR9(x, y, z, d) EXPAND_SEQUENCE_PAIR8(x, y, z, d)##d x##9##y##9##z
#define EXPAND_SEQUENCE_PAIR10(x, y, z, d) EXPAND_SEQUENCE_PAIR9(x, y, z, d)##d x##10##y##10##z
#define EXPAND_SEQUENCE_PAIR11(x, y, z, d) EXPAND_SEQUENCE_PAIR10(x, y, z, d)##d x##11##y##11##z
#define EXPAND_SEQUENCE_PAIR12(x, y, z, d) EXPAND_SEQUENCE_PAIR11(x, y, z, d)##d x##12##y##12##z
#define EXPAND_SEQUENCE_PAIR13(x, y, z, d) EXPAND_SEQUENCE_PAIR12(x, y, z, d)##d x##13##y##13##z
#define EXPAND_SEQUENCE_PAIR14(x, y, z, d) EXPAND_SEQUENCE_PAIR13(x, y, z, d)##d x##14##y##14##z
#define EXPAND_SEQUENCE_PAIR15(x, y, z, d) EXPAND_SEQUENCE_PAIR14(x, y, z, d)##d x##15##y##15##z
#define EXPAND_SEQUENCE_PAIR16(x, y, z, d) EXPAND_SEQUENCE_PAIR15(x, y, z, d)##d x##16##y##16##z
#define EXPAND_SEQUENCE_PAIR17(x, y, z, d) EXPAND_SEQUENCE_PAIR16(x, y, z, d)##d x##17##y##17##z
#define EXPAND_SEQUENCE_PAIR18(x, y, z, d) EXPAND_SEQUENCE_PAIR17(x, y, z, d)##d x##18##y##18##z
#define EXPAND_SEQUENCE_PAIR19(x, y, z, d) EXPAND_SEQUENCE_PAIR18(x, y, z, d)##d x##19##y##19##z
#define EXPAND_SEQUENCE_PAIR20(x, y, z, d) EXPAND_SEQUENCE_PAIR19(x, y, z, d)##d x##20##y##20##z
#define EXPAND_SEQUENCE_PAIR21(x, y, z, d) EXPAND_SEQUENCE_PAIR20(x, y, z, d)##d x##21##y##21##z
#define EXPAND_SEQUENCE_PAIR22(x, y, z, d) EXPAND_SEQUENCE_PAIR21(x, y, z, d)##d x##22##y##22##z
#define EXPAND_SEQUENCE_PAIR23(x, y, z, d) EXPAND_SEQUENCE_PAIR22(x, y, z, d)##d x##23##y##23##z
#define EXPAND_SEQUENCE_PAIR24(x, y, z, d) EXPAND_SEQUENCE_PAIR23(x, y, z, d)##d x##24##y##24##z
#define EXPAND_SEQUENCE_PAIR25(x, y, z, d) EXPAND_SEQUENCE_PAIR24(x, y, z, d)##d x##25##y##25##z
#define EXPAND_SEQUENCE_PAIR26(x, y, z, d) EXPAND_SEQUENCE_PAIR25(x, y, z, d)##d x##26##y##26##z
#define EXPAND_SEQUENCE_PAIR27(x, y, z, d) EXPAND_SEQUENCE_PAIR26(x, y, z, d)##d x##27##y##27##z
#define EXPAND_SEQUENCE_PAIR28(x, y, z, d) EXPAND_SEQUENCE_PAIR27(x, y, z, d)##d x##28##y##28##z
#define EXPAND_SEQUENCE_PAIR29(x, y, z, d) EXPAND_SEQUENCE_PAIR27(x, y, z, d)##d x##29##y##29##z
#define EXPAND_SEQUENCE_PAIR30(x, y, z, d) EXPAND_SEQUENCE_PAIR29(x, y, z, d)##d x##30##y##30##z
#define EXPAND_SEQUENCE_PAIR(x, y, z, i, d) EXPAND_SEQUENCE_PAIR##i(x, y, z, d)

#define EXPAND_DEFINITIONS1(x) x##(1)
#define EXPAND_DEFINITIONS2(x)                                                                                         \
    EXPAND_DEFINITIONS1(x)                                                                                             \
    x##(2)
#define EXPAND_DEFINITIONS3(x)                                                                                         \
    EXPAND_DEFINITIONS2(x)                                                                                             \
    x##(3)
#define EXPAND_DEFINITIONS4(x)                                                                                         \
    EXPAND_DEFINITIONS3(x)                                                                                             \
    x##(4)
#define EXPAND_DEFINITIONS5(x)                                                                                         \
    EXPAND_DEFINITIONS4(x)                                                                                             \
    x##(5)
#define EXPAND_DEFINITIONS6(x)                                                                                         \
    EXPAND_DEFINITIONS5(x)                                                                                             \
    x##(6)
#define EXPAND_DEFINITIONS7(x)                                                                                         \
    EXPAND_DEFINITIONS6(x)                                                                                             \
    x##(7)
#define EXPAND_DEFINITIONS8(x)                                                                                         \
    EXPAND_DEFINITIONS7(x)                                                                                             \
    x##(8)
#define EXPAND_DEFINITIONS9(x)                                                                                         \
    EXPAND_DEFINITIONS8(x)                                                                                             \
    x##(9)
#define EXPAND_DEFINITIONS10(x)                                                                                        \
    EXPAND_DEFINITIONS9(x)                                                                                             \
    x##(10)
#define EXPAND_DEFINITIONS11(x)                                                                                        \
    EXPAND_DEFINITIONS10(x)                                                                                            \
    x##(11)
#define EXPAND_DEFINITIONS12(x)                                                                                        \
    EXPAND_DEFINITIONS11(x)                                                                                            \
    x##(12)
#define EXPAND_DEFINITIONS13(x)                                                                                        \
    EXPAND_DEFINITIONS12(x)                                                                                            \
    x##(13)
#define EXPAND_DEFINITIONS14(x)                                                                                        \
    EXPAND_DEFINITIONS13(x)                                                                                            \
    x##(14)
#define EXPAND_DEFINITIONS15(x)                                                                                        \
    EXPAND_DEFINITIONS14(x)                                                                                            \
    x##(15)
#define EXPAND_DEFINITIONS16(x)                                                                                        \
    EXPAND_DEFINITIONS15(x)                                                                                            \
    x##(16)
#define EXPAND_DEFINITIONS17(x)                                                                                        \
    EXPAND_DEFINITIONS16(x)                                                                                            \
    x##(17)
#define EXPAND_DEFINITIONS18(x)                                                                                        \
    EXPAND_DEFINITIONS17(x)                                                                                            \
    x##(18)
#define EXPAND_DEFINITIONS19(x)                                                                                        \
    EXPAND_DEFINITIONS18(x)                                                                                            \
    x##(19)
#define EXPAND_DEFINITIONS20(x)                                                                                        \
    EXPAND_DEFINITIONS19(x)                                                                                            \
    x##(20)
#define EXPAND_DEFINITIONS21(x)                                                                                        \
    EXPAND_DEFINITIONS20(x)                                                                                            \
    x##(21)
#define EXPAND_DEFINITIONS22(x)                                                                                        \
    EXPAND_DEFINITIONS21(x)                                                                                            \
    x##(22)
#define EXPAND_DEFINITIONS23(x)                                                                                        \
    EXPAND_DEFINITIONS22(x)                                                                                            \
    x##(23)
#define EXPAND_DEFINITIONS24(x)                                                                                        \
    EXPAND_DEFINITIONS23(x)                                                                                            \
    x##(24)
#define EXPAND_DEFINITIONS25(x)                                                                                        \
    EXPAND_DEFINITIONS24(x)                                                                                            \
    x##(25)
#define EXPAND_DEFINITIONS26(x)                                                                                        \
    EXPAND_DEFINITIONS25(x)                                                                                            \
    x##(26)
#define EXPAND_DEFINITIONS27(x)                                                                                        \
    EXPAND_DEFINITIONS26(x)                                                                                            \
    x##(27)
#define EXPAND_DEFINITIONS28(x)                                                                                        \
    EXPAND_DEFINITIONS27(x)                                                                                            \
    x##(28)
#define EXPAND_DEFINITIONS29(x)                                                                                        \
    EXPAND_DEFINITIONS28(x)                                                                                            \
    x##(29)
#define EXPAND_DEFINITIONS30(x)                                                                                        \
    EXPAND_DEFINITIONS29(x)                                                                                            \
    x##(30)
#define EXPAND_DEFINITIONS(x, i) EXPAND_DEFINITIONS##i(x)

#define COMMA ,
#define PARENTHESIS_OPEN (
#define PARENTHESIS_CLOSE )