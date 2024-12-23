/***************************************************************************
                         C_IHP.h  -  description
                   --------------------------------------
    begin                    : Fri Mar 9 2007
    copyright              : (C) 2007 by Gunther Kraut
    email                     : gn.kraut@t-online.de
    spice4qucs code added  Fri. 2 March 2015
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

#ifndef C_IHP_H
#define C_IHP_H

#include "components/component.h"

class C_IHP: public Component
{
public:
    C_IHP();
    ~C_IHP();

    Component* newOne();
    static Element* info(QString&, char* &, bool getNewOne = false);

protected:
    QString netlist();
    QString spice_netlist(bool isXyce = false, bool isCdl = false);
    virtual QString cdl_netlist();
};

#endif // C_IHP_H
