/***************************************************************************
                          NPN_SPICE.h  -  description
                      --------------------------------------
    begin                     : Fri Mar 9 2007
    copyright                 : (C) 2007 by Gunther Kraut
    email                     : gn.kraut@t-online.de
    spice4qucs code added  Thurs. 28 May 2015
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

#ifndef BJT_IHP_H
#define BJT_IHP_H

#include "components/component.h"

class BJT_IHP : public MultiViewComponent
{
public:
    BJT_IHP();
    ~BJT_IHP();

    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne = false);
    static Element* info_l(QString&, char* &, bool getNewOne = false);
    static Element* info_v(QString&, char* &, bool getNewOne = false);

protected:
    void createSymbol();
    QString netlist();
    QString spice_netlist(bool isXyce = false, bool isCdl = false);
    virtual QString cdl_netlist();
};

#endif // NPN_SPICE_H
