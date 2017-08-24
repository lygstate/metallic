#include <cstdint>

extern "C" {

__int128 __ashlti3(__int128 a, int shift)
{
    std::uint64_t array[2];
    __builtin_memcpy(array, &a, sizeof(__int128));

    if (shift & 64)
    {
        array[1] = array[0] << shift;
        array[0] = 0;
    }
    else
    {
        array[1] = array[1] << shift | array[0] >> -shift;
        array[0] <<= shift;
    }

    __builtin_memcpy(&a, array, sizeof(__int128));
    return a;
}

} // extern "C"

namespace {

struct Single
{
    std::uint32_t mantissa: 23;
    unsigned int exp : 8;
    unsigned int sign: 1;

    Single(float);
};

Single::Single(float real)
{
    __builtin_memcpy(this, &real, sizeof(float));
}

struct Double
{
    std::uint64_t mantissa: 52;
    unsigned int exp : 11;
    unsigned int sign: 1;

    Double(double);
};

Double::Double(double real)
{
    __builtin_memcpy(this, &real, sizeof(double));
}

struct Tetra
{
    [[gnu::mode(TF)]] typedef float Real;

    unsigned __int128 mantissa: 112;
    unsigned int exp : 15;
    unsigned int sign:  1;

    Tetra(Real);
    Tetra(Single);
    Tetra(Double);

    Tetra(std::uint32_t);
    Tetra(std::uint64_t);

    Tetra(std::int32_t);
    Tetra(std::int64_t);

    operator Real() const;
    explicit operator bool() const;

    __int128 bits() const;
    bool isnan() const;
};

Tetra::Tetra(Real real)
{
    __builtin_memcpy(this, &real, sizeof(Tetra));
}

Tetra::Tetra(Single object)
  : mantissa(static_cast<unsigned __int128>(object.mantissa) << (112 - 23)),
    exp(object.exp == 0xFF ? 0xFF : 0x3FFF - 0x7F + object.exp),
    sign(object.sign)
{
    if (object.exp == 0) switch (int zeros = __builtin_clz(object.mantissa))
    {
        case 32:
            exp = 0;
            break;
        default:
            exp -= zeros - 9;
            mantissa <<= zeros - 8;
    }
}

Tetra::Tetra(Double object)
  : mantissa(static_cast<unsigned __int128>(object.mantissa) << (112 - 52)),
    exp(object.exp == 0x7FF ? 0x7FF : 0x3FFF - 0x3FF + object.exp),
    sign(object.sign)
{
    if (object.exp == 0) switch (int zeros = __builtin_clzll(object.mantissa))
    {
        case 64:
            exp = 0;
            break;
        default:
            exp -= zeros - 12;
            mantissa <<= zeros - 11;
    }
}

Tetra::Tetra(std::uint32_t integer)
  : mantissa(static_cast<unsigned __int128>(integer) << (112 - 32 + 1 + __builtin_clz(integer))),
    exp((0x401E - __builtin_clz(integer)) * !!integer),
    sign(0)
{}

Tetra::Tetra(std::uint64_t integer)
  : mantissa(static_cast<unsigned __int128>(integer) << (112 - 64 + 1 + __builtin_clzll(integer))),
    exp((0x403E - __builtin_clzll(integer)) * !!integer),
    sign(0)
{}

Tetra::Tetra(std::int32_t integer)
  : Tetra(std::uint32_t(integer < 0 ? -integer : integer))
{
    sign = integer < 0;
}

Tetra::Tetra(std::int64_t integer)
  : Tetra(std::uint64_t(integer < 0 ? -integer : integer))
{
    sign = integer < 0;
}

Tetra::operator Real() const
{
    Real result;
    __builtin_memcpy(&result, this, sizeof(Tetra));
    return result;
}

Tetra::operator bool() const
{
    return exp || mantissa;
}

__int128 Tetra::bits() const
{
    __int128 result;
    __builtin_memcpy(&result, this, sizeof(Tetra));
    return result;
}

bool Tetra::isnan() const
{
    return exp == 0x7FFF && mantissa;
}

} // namespace

namespace std {

bool isunordered(Tetra a, Tetra b)
{
    return Tetra(a).isnan() || Tetra(b).isnan();
}

} // namespace std

bool operator==(Tetra a, Tetra b)
{
    return (a.bits() == b.bits() && !a.isnan()) || !(a || b);
}

bool operator<(Tetra a, Tetra b)
{
    return !std::isunordered(a, b) && ((a.bits() < b.bits()) != (a.sign && b.sign)) && (a || b);
}

bool operator<=(Tetra a, Tetra b)
{
    return (!std::isunordered(a, b) && ((a.bits() <= b.bits()) != (a.sign && b.sign))) || !(a || b);
}

bool operator!=(Tetra a, Tetra b) { return !(a == b); }
bool operator> (Tetra a, Tetra b) { return b < a; }
bool operator>=(Tetra a, Tetra b) { return b <= a; }

Tetra operator-(Tetra a)
{
    a.sign ^= 1;
    return a;
}

extern "C" {

int __unordtf2(Tetra::Real a, Tetra::Real b) { return std::isunordered(Tetra(a), Tetra(b)); }

int __eqtf2(Tetra::Real a, Tetra::Real b) { return Tetra(a) == Tetra(b); }
int __netf2(Tetra::Real a, Tetra::Real b) { return Tetra(a) != Tetra(b); }
int __lttf2(Tetra::Real a, Tetra::Real b) { return Tetra(a) < Tetra(b); }
int __gttf2(Tetra::Real a, Tetra::Real b) { return Tetra(a) > Tetra(b); }
int __letf2(Tetra::Real a, Tetra::Real b) { return Tetra(a) <= Tetra(b); }
int __getf2(Tetra::Real a, Tetra::Real b) { return Tetra(a) >= Tetra(b); }

Tetra::Real __negtf2(Tetra::Real a) { return -Tetra(a); }

Tetra::Real __extendsftf2(float a)  { return Tetra(Single(a)); }
Tetra::Real __extenddftf2(double a) { return Tetra(Double(a)); }
Tetra::Real __floatsitf(std::int32_t a) { return Tetra(a); }
Tetra::Real __floattitf(std::int64_t a) { return Tetra(a); }
Tetra::Real __floatunsitf(std::uint32_t a) { return Tetra(a); }
Tetra::Real __floatuntitf(std::uint64_t a) { return Tetra(a); }

} // extern "C"
