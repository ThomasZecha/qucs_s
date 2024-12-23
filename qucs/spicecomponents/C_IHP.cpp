/***************************************************************************
                         C_IHP.cpp  -  description
                   --------------------------------------
    begin                    : Fri Mar 9 2007
    copyright              : (C) 2007 by Gunther Kraut
    email                     : gn.kraut@t-online.de
    spice4qucs code added  Fri. 3 April 2015
    copyright              : (C) 2015 by Mike Brinson
    email                    : mbrin72043@yahoo.co.uk

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "C_IHP.h"
#include "node.h"
#include "misc.h"
#include "extsimkernels/spicecompat.h"


C_IHP::C_IHP()
{
    Description = QObject::tr("SPICE C:\nMultiple line ngspice or Xyce C specifications allowed using \"+\" continuation lines.\nLeave continuation lines blank when NOT in use.");
    Simulator = spicecompat::simSpice;

    Lines.append(new qucs::Line( -4, -11, -4, 11, QPen(Qt::darkRed, 3)));
    Lines.append(new qucs::Line(  4, -11,  4, 11, QPen(Qt::darkRed, 3)));
    Lines.append(new qucs::Line(-30,  0, -4,  0,  QPen(Qt::darkBlue, 2)));
    Lines.append(new qucs::Line(  4,  0, 30,  0,   QPen(Qt::darkBlue, 2)));

    Ports.append(new Port( 30,  0));
    Ports.append(new Port(-30,  0));

    x1 = -30; y1 = -13;
    x2 =  30; y2 =  13;

    tx = x1+4;
    ty = y2+4;
    Model = "C_IHP";
    SpiceModel = "C";
    Name  = "C";

    Props.append(new Property("model", "cap_cmim", true, "[cap_cmim,cparasitic,cap_rfcmim] modelname"));
    Props.append(new Property("Letter", "X", false, ""));
    Props.append(new Property("W", "10u", true, "Width"));
    Props.append(new Property("L", "10u", true, "Length"));
// Here calculate an initial value based on resistor type and its W and L
    Props.append(new Property("C", "value", true, "Capacitance value"));

    rotate();  // fix historical flaw
}

C_IHP::~C_IHP()
{
}

Component* C_IHP::newOne()
{
    return new C_IHP();
}

Element* C_IHP::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
    Name = QObject::tr("MiM C");
    BitmapFile = const_cast<char*>("C_SPICE");

    if (getNewOne)
    {
        return new C_IHP();
    }

    return nullptr;
}

QString C_IHP::netlist()
{
    return QString("");
}

QString C_IHP::spice_netlist(bool isXyce, bool isCdl /* = false */)
{
    Q_UNUSED(isXyce);
    Q_UNUSED(isCdl);

    QString ltr = getProperty("Letter")->Value;
    QString s = spicecompat::check_refdes(Name, ltr);
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
    s += "\n";

    return s;
}

QString C_IHP::cdl_netlist()
{
    return spice_netlist(false, true);
}
