/***************************************************************************
                         NPN_SPICE.cpp  -  description
                   --------------------------------------
    begin                     : Fri Mar 9 2007
    copyright                 : (C) 2007 by Gunther Kraut
    email                     : gn.kraut@t-online.de
    spice4qucs code added  Thur. 28 May 2015
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

#include "BJT_IHP.h"
#include "node.h"
#include "misc.h"
#include "extsimkernels/spicecompat.h"


BJT_IHP::BJT_IHP()
{
    Description = QObject::tr("Q(NPN) BJT:\nMultiple line ngspice or Xyce Q model specifications allowed using \"+\" continuation lines.\nLeave continuation lines blank when NOT in use.");
    Simulator = spicecompat::simSpice;

    x1 = -30; y1 = -30;
    x2 =   4; y2 =  30;

    tx = x1+4;
    ty = y2+4;

    Model = "npn13g2";
    SpiceModel = "Q";
    Name  = "Q";

    Props.append(new Property("Letter", "X", false, "[Q, X] BJT type"));
    Props.append(new Property("model", "npn13g2", true, "[npn13g2,npn13g2l,npn13g2v]"));
    Props.append(new Property("Nx", "1", true, "Number of emmiters"));
    Props.append(new Property("El", "", false, "Emiter length"));

    createSymbol();

}


void BJT_IHP::createSymbol()
{

    Lines.append(new qucs::Line(-10, -15, -10, 15, QPen(Qt::darkRed, 3)));

    Lines.append(new qucs::Line(-30,  0, -20,  0, QPen(Qt::darkBlue, 3)));
    Lines.append(new qucs::Line(-20,  0, -10,  0, QPen(Qt::darkRed, 3)));

    Lines.append(new qucs::Line(-10, -5,  0, -15, QPen(Qt::darkRed, 3)));

    Lines.append(new qucs::Line(  0, -15,  0, -20, QPen(Qt::darkRed, 3)));
    Lines.append(new qucs::Line(  0, -20,  0, -30, QPen(Qt::darkBlue, 3)));

    Lines.append(new qucs::Line(-10,  5,  0, 15, QPen(Qt::darkRed, 3)));

    Lines.append(new qucs::Line(  0, 15,  0, 20, QPen(Qt::darkRed, 3)));
    Lines.append(new qucs::Line(  0, 20,  0, 30, QPen(Qt::darkBlue, 3)));

    Lines.append(new qucs::Line( -6, 15,  0, 15, QPen(Qt::darkRed, 3)));
    Lines.append(new qucs::Line(  0,  9,  0, 15, QPen(Qt::darkRed, 3)));

    Lines.append(new qucs::Line(  9,  0, 30,  0, QPen(Qt::darkRed, 2)));
    Lines.append(new qucs::Line(  9, -7,  9,  7, QPen(Qt::darkRed, 3)));

    Ports.append(new Port(0, -30));
    Ports.append(new Port(-30, 0));
    Ports.append(new Port(  0, 30));
    Ports.append(new Port( 30,  0)); // substrate
}

BJT_IHP::~BJT_IHP()
{
}

Component* BJT_IHP::newOne()
{
    return new BJT_IHP();
}

Element* BJT_IHP::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
    Name = QObject::tr("npn13g2");
    BitmapFile = const_cast<char*>("NPN4_SPICE");

    if (getNewOne)
    {
        return new BJT_IHP();
    }

    return nullptr;
}

Element* BJT_IHP::info_l(QString& Name, char* &BitmapFile, bool getNewOne)
{
    Name = QObject::tr("npn13g2l");
    BitmapFile = const_cast<char*>("NPN4_SPICE");

    if (getNewOne)
    {
        auto p = new BJT_IHP();
        p->Props.at(1)->Value = "npn13g2l";
        p->Props.at(3)->display = true;
        p->Props.at(3)->Value = "1";
        p->recreate(0);
        return p;
    }

    return nullptr;
}

Element* BJT_IHP::info_v(QString& Name, char* &BitmapFile, bool getNewOne)
{
    Name = QObject::tr("npn13g2v");
    BitmapFile = const_cast<char*>("NPN4_SPICE");

    if (getNewOne)
    {
        auto p = new BJT_IHP();
        p->Props.at(1)->Value = "npn13g2v";
        p->recreate(0);
        return p;
    }

    return nullptr;
}


QString BJT_IHP::netlist()
{
    return QString("");
}

QString BJT_IHP::spice_netlist(bool isXyce, bool isCdl /* = false */)
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
    QString Nx = getProperty("Nx")->Value;
    QString El = getProperty("El")->Value;

    if (model.length() > 0)
    {
        s += QString(" %1").arg(model);
    }
    if (Nx.length() > 0)
    {
        s += QString(" Nx=%1").arg(Nx);
    }
    if (El.length() > 0)
    {
        s += QString(" El=%1").arg(El);
    }

    s += "\n";

    return s;
}

QString BJT_IHP::cdl_netlist()
{
    return spice_netlist(false, true);
}
