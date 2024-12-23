/***************************************************************************
                         DIODE_IHP.cpp  -  description
                   --------------------------------------
    begin                    : Fri Mar 9 2007
    copyright              : (C) 2007 by Gunther Kraut
    email                     : gn.kraut@t-online.de
    spice4qucs code added  Wed. 27 May 2015
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

#include "DIODE_IHP.h"
#include "node.h"
#include "misc.h"
#include "extsimkernels/spicecompat.h"


DIODE_IHP::DIODE_IHP()
{
    Description = QObject::tr("SPICE D:\nMultiple line ngspice or Xyce D model specifications allowed using \"+\" continuation lines.\nLeave continuation lines blank when NOT in use.");
    Simulator = spicecompat::simSpice;

    Lines.append(new qucs::Line(-30,  0, -20,  0, QPen(Qt::darkBlue, 3)));
    Lines.append(new qucs::Line( -20, 0, -6,   0, QPen(Qt::darkRed, 3)));
    Lines.append(new qucs::Line(  6,  0, 20,   0, QPen(Qt::darkRed, 3)));
    Lines.append(new qucs::Line( 20,  0, 30,   0, QPen(Qt::darkBlue, 3)));


    Lines.append(new qucs::Line( -6, -9, -6,  9, QPen(Qt::darkRed, 3)));
    Lines.append(new qucs::Line(  6, -9,  6,  9, QPen(Qt::darkRed, 3)));
    Lines.append(new qucs::Line( -6, -9,  6,  0, QPen(Qt::darkRed, 3)));
    Lines.append(new qucs::Line( -6,  9,  6,  0, QPen(Qt::darkRed, 3)));

    Ports.append(new Port(-30, 0));
    Ports.append(new Port( 30, 0));

    x1 = -30; y1 = -11;
    x2 =  30; y2 =  11;

    tx = x1+4;
    ty = y2+4;

    Model = "DIODE_IHP";
    SpiceModel = "D";
    Name  = "D";

    Props.append(new Property("model", "dantenna", true, "[dantenna,dpantenna]modelname"));
    Props.append(new Property("Letter", "X", false, "[D,X]"));
    Props.append(new Property("W", "0.78u", true, "Width"));
    Props.append(new Property("L", "0.78u", true, "Length"));
}

DIODE_IHP::~DIODE_IHP()
{
}

Component* DIODE_IHP::newOne()
{
    return new DIODE_IHP();
}

Element* DIODE_IHP::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
    Name = QObject::tr("Diode");
    BitmapFile = const_cast<char*>("DIODE_SPICE");

    if (getNewOne)
    {
        return new DIODE_IHP();
    }

    return nullptr;
}

QString DIODE_IHP::netlist()
{
    return QString("");
}

QString DIODE_IHP::spice_netlist(bool isXyce, bool isCdl /* = false */)
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

    QString D = getProperty("model")->Value;
    QString W = getProperty("W")->Value;
    QString L = getProperty("L")->Value;

    if (D.length() > 0)
    {
        s += QString("%1 ").arg(D);
    }
    if (W.length() > 0 )
    {
        s += QString("W=%1 ").arg(W);
    }
    if (L.length() > 0 )
    {
        s += QString("L=%1 ").arg(L);
    }
    s += "\n";
    return s;
}

QString DIODE_IHP::cdl_netlist()
{
    return spice_netlist(false, true);
}
