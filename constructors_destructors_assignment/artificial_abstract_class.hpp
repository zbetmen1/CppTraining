/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright 2014  <copyright holder> <email>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef ARTIFICIAL_ABSTRACT_CLASS_H
#define ARTIFICIAL_ABSTRACT_CLASS_H

class artificial_abstract_class
{
public:
  /**
   * @brief This is one big exception in C++. First when you want some class to be abstract and
   * you don't have any pure virtual methods in it, what you can do is specify pure virtual destructor.
   * This is fine, but the odd part is that you MUST PROVIDE DEFINITION FOR PURE VIRTUAL DESTRUCTOR.
   * 
   */  
  virtual ~artificial_abstract_class() = 0;
};

#endif // ARTIFICIAL_ABSTRACT_CLASS_H
