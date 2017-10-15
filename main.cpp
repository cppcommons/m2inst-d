#include <QtCore>
#include <QtNetwork>

#include <stdio.h>
#include <stdlib.h>

#include "duktape.h"

#include <iostream>
#include <string>
#include <sstream>
#include <bitset>

QString str64bit(quint64 x) {
    std::stringstream ss;
    ss << static_cast<std::bitset<64> >(x);
    std::string x_bin = ss.str();
    return QString::fromLatin1(x_bin.c_str());
}

QString JX9_PROG = R"***(
print(11+22)
print(add2(6, 7))
)***";

/* Being an embeddable engine, Duktape doesn't provide I/O
 * bindings by default.  Here's a simple one argument print()
 * function.
 */
static duk_ret_t native_print(duk_context *ctx) {
    if (!ctx) return 1;
    printf("%s\n", duk_to_string(ctx, 0));
    return 0;  /* no return value (= undefined) */
}

/* Adder: add argument values. */
static duk_ret_t native_adder(duk_context *ctx) {
    if (!ctx) return DUK_VARARGS;
    int i;
    int n = duk_get_top(ctx);  /* #args */
    double res = 0.0;

    for (i = 0; i < n; i++) {
        res += duk_to_number(ctx, i);
    }

    duk_push_number(ctx, res);
    return 1;  /* one return value */
}

static duk_ret_t native_add2(duk_context *ctx) {
    if (!ctx) return 2;
    //printf("0=%lf\n", duk_to_number(ctx, 0));
    //printf("1=%lf\n", duk_to_number(ctx, 1));
    duk_push_number(ctx, duk_to_number(ctx, 0) + duk_to_number(ctx, 1));
    return 1;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug().noquote() << JX9_PROG.trimmed();

    duk_context *ctx = duk_create_heap_default();
    duk_eval_string(ctx, "1+2");
    printf("1+2=%d\n", (int) duk_get_int(ctx, -1));

    duk_push_c_function(ctx, native_print, native_print(NULL) /*nargs*/);
    duk_put_global_string(ctx, "print");
    duk_push_c_function(ctx, native_adder, native_adder(NULL));
    duk_put_global_string(ctx, "adder");
    duk_push_c_function(ctx, native_add2, native_add2(NULL));
    duk_put_global_string(ctx, "add2");
    duk_pop(ctx);

    duk_eval_string_noresult(ctx, "print('2+3=' + adder(2, 3));");
    //std::string s = JX9_PROG.trimmed().toUtf8().constData();
    //qDebug() << s.c_str();
    //duk_eval_string_noresult(ctx, s.c_str());
    duk_eval_string_noresult(ctx, JX9_PROG.trimmed().toUtf8().constData());

    ///duk_push_string(ctx, "XXX");
    duk_push_string(ctx, "Zm9v");
    duk_base64_decode(ctx, -1);  /* buffer */
    //printf("base-64 decoded: %s\n", duk_to_string(ctx, -1));
    printf("coerced string: %s\n", duk_buffer_to_string(ctx, -1));
    duk_pop(ctx);

    duk_push_object(ctx);
    duk_push_int(ctx, 42);
    duk_put_prop_string(ctx, -2, "meaningOfLife");
    printf("JSON encoded: %s\n", duk_json_encode(ctx, -1));
    duk_pop(ctx);

    duk_int_t rc;
    duk_push_string(ctx, "add2");
    duk_eval(ctx);
    duk_push_int(ctx, 2);
    duk_push_number(ctx, 3);
    rc = duk_pcall(ctx, 2);
    if (rc == DUK_EXEC_SUCCESS) {
        printf("2+3=%ld\n", (long) duk_get_int(ctx, -1));
    } else {
        printf("error: %s\n", duk_to_string(ctx, -1));
    }
    duk_pop(ctx);

    unsigned long long ull = UINT64_MAX;
    QString sp;
    sp.sprintf("%llu", ull);
    qDebug().noquote() << sp;
    duk_push_string(ctx, sp.toUtf8().constData());
    duk_eval(ctx);
    QString evaled = QString::fromUtf8(duk_to_string(ctx, -1));
    printf("eval'ed=%s\n", evaled.toLocal8Bit().constData());
    quint64 evaled2 = strtoull(evaled.toUtf8().constData(), NULL, 10);
    printf("eval'ed2=%llu\n", evaled2);
    std::cout << static_cast<std::bitset<64> >(evaled2) << std::endl;
    qDebug() << str64bit(evaled2);
    duk_pop(ctx);

    auto f = [=](quint64 x) -> bool {
        QString sp;
        sp.sprintf("%llu", x);
        qDebug().noquote() << "x=" << sp;
        duk_push_string(ctx, sp.toUtf8().constData());
        duk_eval(ctx);
        QString evaled = QString::fromUtf8(duk_to_string(ctx, -1));
        printf("eval'ed=%s\n", evaled.toLocal8Bit().constData());
        quint64 evaled2 = strtoull(evaled.toUtf8().constData(), NULL, 10);
        printf("eval'ed2=%llu\n", evaled2);
        std::cout << static_cast<std::bitset<64> >(evaled2) << std::endl;
        qDebug() << str64bit(evaled2);
        duk_pop(ctx);
        return true;
    };

    for (quint64 i=0; i<5; i++) {
        bool b = f(i);
    }


    duk_destroy_heap(ctx);
    return 0;
}

#if 0x0

#include <iostream>
#include <bitset>

int main() {
    unsigned x = 11;
    std::cout << static_cast<std::bitset<8> >(x) << std::endl;
}

#include <iostream>
#include <sstream>
#include <bitset>

int main() {
    unsigned x = 11;
    std::stringstream ss;
    ss << static_cast<std::bitset<8> >(x);
    std::string x_bin = ss.str();
    std::cout << x_bin << std::endl;
}

#endif
