/***************************************************************************
                         NMOS_SPICE.cpp  -  description
                   --------------------------------------
    begin                     : Fri Mar 9 2007
    copyright                 : (C) 2007 by Gunther Kraut
    email                     : gn.kraut@t-online.de
    spice4qucs code added  Sat. 30 May 2015
    copyright                 : (C) 2015 by Mike Brinson
    email                     : mbrin72043@yahoo.co.uk

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "MOS_IHP.h"
#include "node.h"
#include "misc.h"
#include "extsimkernels/spicecompat.h"


MOS_IHP::MOS_IHP()
{
    Description = QObject::tr("4-pin MOS:\nMultiple line ngspice or Xyce M model specifications allowed using \"+\" continuation lines.\nLeave continuation lines blank when NOT in use.");
    Simulator = spicecompat::simSpice;

    Props.append(new Property("Letter", "X", false, "SPICE letter"));
    Props.append(new Property("type", "nmos", false, "Channel type"));
    Props.append(new Property("model", "sg13_lv_nmos", true, "[sg13_lv_nmos,sg13_hv_nmos]"));
    Props.append(new Property("W", "1u", true, "Width"));
    Props.append(new Property("L", "1u", true, "Length"));
    Props.append(new Property("ng", "1", false, "no. gatess"));
    Props.append(new Property("m", "1", false, "no. devices"));

    createSymbol();

    x1 = -30; y1 = -30;
    x2 =   4; y2 =  30;

    tx = x1+4;
    ty = y2+4;

    Model = "MOS_IHP";
    SpiceModel = "M";
    Name  = "M";
}

MOS_IHP::~MOS_IHP()
{
}

Component* MOS_IHP::newOne()
{
    return new MOS_IHP();
}

Element* MOS_IHP::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
    Name = QObject::tr("LV N-mos");
    BitmapFile = const_cast<char*>("NMOS_SPICE_4");

    if (getNewOne)
    {
        MOS_IHP *p = new MOS_IHP();
        p->Props.at(2)->Value = "sg13_lv_nmos";
        p->recreate(0);
        return p;
    }

    return nullptr;
}

Element* MOS_IHP::info_hv(QString& Name, char* &BitmapFile, bool getNewOne)
{
    Name = QObject::tr("HV N-mos");
    BitmapFile = const_cast<char*>("NMOS_SPICE_4");

    if (getNewOne)
    {
        MOS_IHP *p = new MOS_IHP();
        p->Props.at(2)->Value = "sg13_hv_nmos";
        p->recreate(0);
        return p;
    }

    return nullptr;
}

Element* MOS_IHP::info_pmos(QString& Name, char* &BitmapFile, bool getNewOne)
{
    Name = QObject::tr("LV P-mos");
    BitmapFile = const_cast<char*>("PMOS_SPICE_4");

    if (getNewOne)
    {
        MOS_IHP *p = new MOS_IHP();
        p->Props.at(1)->Value = "pmos";
        p->Props.at(2)->Value = "sg13_lv_pmos";
        p->recreate(0);
        return p;
    }

    return nullptr;
}

Element* MOS_IHP::info_hv_pmos(QString& Name, char* &BitmapFile, bool getNewOne)
{
    Name = QObject::tr("HV P-mos");
    BitmapFile = const_cast<char*>("PMOS_SPICE_4");

    if (getNewOne)
    {
        MOS_IHP *p = new MOS_IHP();
        p->Props.at(1)->Value = "pmos";
        p->Props.at(2)->Value = "sg13_hv_pmos";
        p->recreate(0);
        return p;
    }

    return nullptr;
}

QString MOS_IHP::netlist()
{
    return QString("");
}

void MOS_IHP::createSymbol()
{
    Lines.append(new qucs::Line(-14, -13, -14, 13, QPen(Qt::darkRed, 2)));

    Lines.append(new qucs::Line(-30,  0, -20,  0, QPen(Qt::darkBlue, 2)));
    Lines.append(new qucs::Line(-20,  0, -14,  0, QPen(Qt::darkRed, 2)));

    Lines.append(new qucs::Line(-10, -11,  0, -11, QPen(Qt::darkRed, 2)));

    Lines.append(new qucs::Line(  0, -11,  0, -20, QPen(Qt::darkRed, 2)));
    Lines.append(new qucs::Line(  0, -20,  0, -30, QPen(Qt::darkBlue, 2)));

    Lines.append(new qucs::Line(-10, 11,  0, 11, QPen(Qt::darkRed, 2)));
    Lines.append(new qucs::Line(  0, 11,  0, 20, QPen(Qt::darkRed, 2)));
    Lines.append(new qucs::Line(  0, 20,  0, 30, QPen(Qt::darkBlue, 2)));

    Lines.append(new qucs::Line(-10, -16, -10, -7, QPen(Qt::darkRed, 2)));
    Lines.append(new qucs::Line(-10,  7, -10, 16, QPen(Qt::darkRed, 2)));

    Lines.append(new qucs::Line(-10, -8, -10,  8, QPen(Qt::darkRed, 2)));
    Lines.append(new qucs::Line( -4, 24,  4, 20, QPen(Qt::darkRed, 2)));

    Ports.append(new Port(  0, -30)); //D
    Ports.append(new Port(-30,  0)); //G
    Ports.append(new Port(  0, 30)); //S
    Ports.append(new Port( 20,  0)); //B
    Lines.append(new qucs::Line( 10,  0, 20,  0, QPen(Qt::darkBlue, 2)));
    Lines.append(new qucs::Line(-10,  0, 10,  0, QPen(Qt::darkRed, 2)));

    if (Props.at(1)->Value == "nmos")
    {
        Lines.append(new qucs::Line( -9,  0, -4, -5, QPen(Qt::darkRed, 2)));
        Lines.append(new qucs::Line( -9,  0, -4,  5, QPen(Qt::darkRed, 2)));
    }
    else
    {
        Lines.append(new qucs::Line( -1,  0, -6, -5, QPen(Qt::darkRed, 2)));
        Lines.append(new qucs::Line( -1,  0, -6,  5, QPen(Qt::darkRed, 2)));
    }
}


QString MOS_IHP::spice_netlist(spicecompat::SpiceDialect dialect /* = spicecompat::SPICEDefault */)
{
    Q_UNUSED(dialect);

    QString s = spicecompat::check_refdes(Name, Props.at(0)->Value);
    for (Port *p1 : Ports)
    {
        QString name = p1->Connection->Name;
        if (name == "gnd")
        {
            name = "0";
        }

        s += " " + name + " ";   // node names
    }

    QString model = getProperty("model")->Value;
    QString W = getProperty("W")->Value;
    QString L = getProperty("L")->Value;
    QString ng = getProperty("ng")->Value;
    QString m = getProperty("m")->Value;


    if (model.length()  > 0)
    {
        s += QString("%1").arg(model);
    }
    if (W.length() > 0)
    {
        s += QString(" W=%1").arg(W);
    }
    if (L.length() > 0)
    {
        s += QString(" L=%1").arg(L);
    }
    if (ng.length() > 0)
    {
        s += QString(" ng=%1").arg(ng);
    }
    if (m.length() > 0)
    {
        s += QString(" m=%1").arg(m);
    }
    s += "\n";

    return s;
}

QString MOS_IHP::cdl_netlist()
{
    return spice_netlist(spicecompat::CDL);
}

