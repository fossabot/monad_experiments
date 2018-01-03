// monadicclutter.cpp : Defines the entry point for the console application.
//

#include <cassert>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <iostream>


struct FormInput { std::string_view value; }; 
struct Index { int value; };
struct Ratio { double value; };

struct Voltage { double value; };
struct VoltageRange { Voltage low; Voltage high; };

Index fromForm(FormInput input) { return { atoi(input.value.data()) }; }
Ratio fromIndex(Index i) {
    return { static_cast<double>(i.value) / 100.0 };
}
auto inRange(VoltageRange range) {
    return [=](Ratio r) { return Voltage{ range.low.value + r.value * (range.high.value - range.low.value) }; };
}
const VoltageRange range{ { 1.0 },{ 10.0 } };
const auto toVoltage = inRange(range);

auto toVoltageString(const std::optional<std::string_view> &arg)
{
    if (!arg) return std::string{"?"};
    const auto input = FormInput{ *arg };
    auto v = toVoltage(fromIndex(fromForm(input)));
    return std::to_string(v.value).substr(0, 3) + "V";
}

int main(const int argc, const char** args)
{
    std::optional<std::string_view> arg;
    if (argc > 1) arg = {args[1]};
    std::cout << toVoltageString(arg) << std::endl;
    assert(toVoltageString({}) == "?");
    assert(toVoltageString({ "90" }) == "9.1V");
    return 0;
}

