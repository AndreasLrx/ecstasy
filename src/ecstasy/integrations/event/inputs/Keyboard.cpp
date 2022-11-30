///
/// @file Keyboard.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-11-17
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include "Keyboard.hpp"
#include <unordered_map>

#define MAP_INPUT(type)                                         \
    {                                                           \
        #type, ecstasy::integration::event::Keyboard::Key::type \
    }

/// @internal
#define ECSTASY_KEY_NAME_CASE(key) \
    case ecstasy::integration::event::Keyboard::Key::key: return stream << #key;

std::ostream &operator<<(std::ostream &stream, const ecstasy::integration::event::Keyboard::Key &key)
{
    switch (key) {
        ECSTASY_KEY_NAME_CASE(Unknown);
        ECSTASY_KEY_NAME_CASE(A);
        ECSTASY_KEY_NAME_CASE(B);
        ECSTASY_KEY_NAME_CASE(C);
        ECSTASY_KEY_NAME_CASE(D);
        ECSTASY_KEY_NAME_CASE(E);
        ECSTASY_KEY_NAME_CASE(F);
        ECSTASY_KEY_NAME_CASE(G);
        ECSTASY_KEY_NAME_CASE(H);
        ECSTASY_KEY_NAME_CASE(I);
        ECSTASY_KEY_NAME_CASE(J);
        ECSTASY_KEY_NAME_CASE(K);
        ECSTASY_KEY_NAME_CASE(L);
        ECSTASY_KEY_NAME_CASE(M);
        ECSTASY_KEY_NAME_CASE(N);
        ECSTASY_KEY_NAME_CASE(O);
        ECSTASY_KEY_NAME_CASE(P);
        ECSTASY_KEY_NAME_CASE(Q);
        ECSTASY_KEY_NAME_CASE(R);
        ECSTASY_KEY_NAME_CASE(S);
        ECSTASY_KEY_NAME_CASE(T);
        ECSTASY_KEY_NAME_CASE(U);
        ECSTASY_KEY_NAME_CASE(V);
        ECSTASY_KEY_NAME_CASE(W);
        ECSTASY_KEY_NAME_CASE(X);
        ECSTASY_KEY_NAME_CASE(Y);
        ECSTASY_KEY_NAME_CASE(Z);
        ECSTASY_KEY_NAME_CASE(Num0);
        ECSTASY_KEY_NAME_CASE(Num1);
        ECSTASY_KEY_NAME_CASE(Num2);
        ECSTASY_KEY_NAME_CASE(Num3);
        ECSTASY_KEY_NAME_CASE(Num4);
        ECSTASY_KEY_NAME_CASE(Num5);
        ECSTASY_KEY_NAME_CASE(Num6);
        ECSTASY_KEY_NAME_CASE(Num7);
        ECSTASY_KEY_NAME_CASE(Num8);
        ECSTASY_KEY_NAME_CASE(Num9);
        ECSTASY_KEY_NAME_CASE(Escape);
        ECSTASY_KEY_NAME_CASE(LControl);
        ECSTASY_KEY_NAME_CASE(LShift);
        ECSTASY_KEY_NAME_CASE(LAlt);
        ECSTASY_KEY_NAME_CASE(LSystem);
        ECSTASY_KEY_NAME_CASE(RControl);
        ECSTASY_KEY_NAME_CASE(RShift);
        ECSTASY_KEY_NAME_CASE(RAlt);
        ECSTASY_KEY_NAME_CASE(RSystem);
        ECSTASY_KEY_NAME_CASE(Menu);
        ECSTASY_KEY_NAME_CASE(LBracket);
        ECSTASY_KEY_NAME_CASE(RBracket);
        ECSTASY_KEY_NAME_CASE(Semicolon);
        ECSTASY_KEY_NAME_CASE(Comma);
        ECSTASY_KEY_NAME_CASE(Period);
        ECSTASY_KEY_NAME_CASE(Quote);
        ECSTASY_KEY_NAME_CASE(Slash);
        ECSTASY_KEY_NAME_CASE(Backslash);
        ECSTASY_KEY_NAME_CASE(Tilde);
        ECSTASY_KEY_NAME_CASE(Equal);
        ECSTASY_KEY_NAME_CASE(Hyphen);
        ECSTASY_KEY_NAME_CASE(Space);
        ECSTASY_KEY_NAME_CASE(Enter);
        ECSTASY_KEY_NAME_CASE(Backspace);
        ECSTASY_KEY_NAME_CASE(Tab);
        ECSTASY_KEY_NAME_CASE(PageUp);
        ECSTASY_KEY_NAME_CASE(PageDown);
        ECSTASY_KEY_NAME_CASE(End);
        ECSTASY_KEY_NAME_CASE(Home);
        ECSTASY_KEY_NAME_CASE(Insert);
        ECSTASY_KEY_NAME_CASE(Delete);
        ECSTASY_KEY_NAME_CASE(Add);
        ECSTASY_KEY_NAME_CASE(Subtract);
        ECSTASY_KEY_NAME_CASE(Multiply);
        ECSTASY_KEY_NAME_CASE(Divide);
        ECSTASY_KEY_NAME_CASE(Left);
        ECSTASY_KEY_NAME_CASE(Right);
        ECSTASY_KEY_NAME_CASE(Up);
        ECSTASY_KEY_NAME_CASE(Down);
        ECSTASY_KEY_NAME_CASE(Numpad0);
        ECSTASY_KEY_NAME_CASE(Numpad1);
        ECSTASY_KEY_NAME_CASE(Numpad2);
        ECSTASY_KEY_NAME_CASE(Numpad3);
        ECSTASY_KEY_NAME_CASE(Numpad4);
        ECSTASY_KEY_NAME_CASE(Numpad5);
        ECSTASY_KEY_NAME_CASE(Numpad6);
        ECSTASY_KEY_NAME_CASE(Numpad7);
        ECSTASY_KEY_NAME_CASE(Numpad8);
        ECSTASY_KEY_NAME_CASE(Numpad9);
        ECSTASY_KEY_NAME_CASE(F1);
        ECSTASY_KEY_NAME_CASE(F2);
        ECSTASY_KEY_NAME_CASE(F3);
        ECSTASY_KEY_NAME_CASE(F4);
        ECSTASY_KEY_NAME_CASE(F5);
        ECSTASY_KEY_NAME_CASE(F6);
        ECSTASY_KEY_NAME_CASE(F7);
        ECSTASY_KEY_NAME_CASE(F8);
        ECSTASY_KEY_NAME_CASE(F9);
        ECSTASY_KEY_NAME_CASE(F10);
        ECSTASY_KEY_NAME_CASE(F11);
        ECSTASY_KEY_NAME_CASE(F12);
        ECSTASY_KEY_NAME_CASE(F13);
        ECSTASY_KEY_NAME_CASE(F14);
        ECSTASY_KEY_NAME_CASE(F15);
        ECSTASY_KEY_NAME_CASE(Pause);
        default: return stream;
    }
}

std::istream &operator>>(std::istream &stream, ecstasy::integration::event::Keyboard::Key &key)
{
    static const std::unordered_map<std::string_view, ecstasy::integration::event::Keyboard::Key> map = {
        MAP_INPUT(Unknown), MAP_INPUT(A), MAP_INPUT(B), MAP_INPUT(C), MAP_INPUT(D), MAP_INPUT(E), MAP_INPUT(F),
        MAP_INPUT(G), MAP_INPUT(H), MAP_INPUT(I), MAP_INPUT(J), MAP_INPUT(K), MAP_INPUT(L), MAP_INPUT(M), MAP_INPUT(N),
        MAP_INPUT(O), MAP_INPUT(P), MAP_INPUT(Q), MAP_INPUT(R), MAP_INPUT(S), MAP_INPUT(T), MAP_INPUT(U), MAP_INPUT(V),
        MAP_INPUT(W), MAP_INPUT(X), MAP_INPUT(Y), MAP_INPUT(Z), MAP_INPUT(Num0), MAP_INPUT(Num1), MAP_INPUT(Num2),
        MAP_INPUT(Num3), MAP_INPUT(Num4), MAP_INPUT(Num5), MAP_INPUT(Num6), MAP_INPUT(Num7), MAP_INPUT(Num8),
        MAP_INPUT(Num9), MAP_INPUT(Escape), MAP_INPUT(LControl), MAP_INPUT(LShift), MAP_INPUT(LAlt), MAP_INPUT(LSystem),
        MAP_INPUT(RControl), MAP_INPUT(RShift), MAP_INPUT(RAlt), MAP_INPUT(RSystem), MAP_INPUT(Menu),
        MAP_INPUT(LBracket), MAP_INPUT(RBracket), MAP_INPUT(Semicolon), MAP_INPUT(Comma), MAP_INPUT(Period),
        MAP_INPUT(Quote), MAP_INPUT(Slash), MAP_INPUT(Backslash), MAP_INPUT(Tilde), MAP_INPUT(Equal), MAP_INPUT(Hyphen),
        MAP_INPUT(Space), MAP_INPUT(Enter), MAP_INPUT(Backspace), MAP_INPUT(Tab), MAP_INPUT(PageUp),
        MAP_INPUT(PageDown), MAP_INPUT(End), MAP_INPUT(Home), MAP_INPUT(Insert), MAP_INPUT(Delete), MAP_INPUT(Add),
        MAP_INPUT(Subtract), MAP_INPUT(Multiply), MAP_INPUT(Divide), MAP_INPUT(Left), MAP_INPUT(Right), MAP_INPUT(Up),
        MAP_INPUT(Down), MAP_INPUT(Numpad0), MAP_INPUT(Numpad1), MAP_INPUT(Numpad2), MAP_INPUT(Numpad3),
        MAP_INPUT(Numpad4), MAP_INPUT(Numpad5), MAP_INPUT(Numpad6), MAP_INPUT(Numpad7), MAP_INPUT(Numpad8),
        MAP_INPUT(Numpad9), MAP_INPUT(F1), MAP_INPUT(F2), MAP_INPUT(F3), MAP_INPUT(F4), MAP_INPUT(F5), MAP_INPUT(F6),
        MAP_INPUT(F7), MAP_INPUT(F8), MAP_INPUT(F9), MAP_INPUT(F10), MAP_INPUT(F11), MAP_INPUT(F12), MAP_INPUT(F13),
        MAP_INPUT(F14), MAP_INPUT(F15), MAP_INPUT(Pause)};

    std::string buffer;
    stream >> buffer;
    key = map.at(buffer);
    return stream;
}