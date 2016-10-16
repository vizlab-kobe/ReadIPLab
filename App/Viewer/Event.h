#pragma once
#include <kvs/glut/Label>
#include <kvs/KeyPressEventListener>
#include <kvs/PaintEventListener>
#include "Data.h"


class PaintEvent : public kvs::PaintEventListener
{
private:
    Data& m_data;
    kvs::glut::Label& m_label;

public:
    PaintEvent( Data& data, kvs::glut::Label& label );
    void update();
};

class KeyPressEvent : public kvs::KeyPressEventListener
{
private:
    Data& m_data;

public:
    KeyPressEvent( Data& data );
    void update( kvs::KeyEvent* event );
};
