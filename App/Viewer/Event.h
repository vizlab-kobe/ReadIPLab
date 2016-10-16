#pragma once
#include <kvs/glut/Label>
#include <kvs/KeyPressEventListener>
#include <kvs/PaintEventListener>
#include <Lib/IPLab.h>
#include <Lib/IPLabList.h>
#include <Lib/Importer.h>
#include "Input.h"


class PaintEvent : public kvs::PaintEventListener
{
private:
    ReadIPLab::IPLab& m_ipl;
    Input& m_input;
    kvs::glut::Label& m_label;

public:
    PaintEvent( ReadIPLab::IPLab& ipl, Input& input, kvs::glut::Label& label );
    void update();
};

class KeyPressEvent : public kvs::KeyPressEventListener
{
private:
    ReadIPLab::IPLabList& m_ipl_list;
    ReadIPLab::IPLab& m_ipl;
    Input& m_input;

public:

    KeyPressEvent( ReadIPLab::IPLabList& ipl_list, ReadIPLab::IPLab& ipl, Input& input );
    void update( kvs::KeyEvent* event );
};
