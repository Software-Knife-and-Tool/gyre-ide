/*******
 **
 ** Copyright (c) 2017, James M. Putnam
 ** All rights reserved.
 **
 ** Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are met:
 **
 ** 1. Redistributions of source code must retain the above copyright notice,
 **    this list of conditions and the following disclaimer.
 **
 ** 2. Redistributions in binary form must reproduce the above copyright
 **    notice, this list of conditions and the following disclaimer in the
 **    documentation and/or other materials provided with the distribution.
 **
 ** 3. Neither the name of the copyright holder nor the names of its
 **    contributors may be used to endorse or promote products derived from
 **    this software without specific prior written permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 ** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 ** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 ** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 ** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 ** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 ** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 ** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 ** POSSIBILITY OF SUCH DAMAGE.
 **
 *******/

/********
 **
 **  TileFrame.cpp: TileFrame implementation
 **
 **/
#include <QFileDialog>
#include <QLabel>
#include <QTextEdit>
#include <QToolBar>
#include <QSplitter>
#include <QString>
#include <QtWidgets>

#include "Tile.h"
#include "canon.h"

namespace logicaide {

namespace {

void scrub_layout(QLayout* layout) {
  QLayoutItem* item;
  QLayout* sublayout;
  QWidget* widget;

  while ((item = layout->takeAt(0))) {
    if ((sublayout = item->layout()) != 0) {/* do the same for sublayout*/}
    else if ((widget = item->widget()) != 0) {widget->hide(); delete widget;}
    else {delete item;}
  }
  
  delete layout;
}
  
} /* anynonymous namespace */

void Tile::split(QFrame* fr) {
  switch (splitState) {
  case UNSPLIT:
    break;
  case HORIZONTAL:
    splitTile = new Tile(tabBar, fr);
    splith();
    break;
  case VERTICAL:
    splitTile = new Tile(tabBar, fr);
    splitv();
    break;
  }
}

void Tile::rebase(QFrame* base) {
  auto size = this->frameSize();

  baseFrame = base;
  baseFrame->setMinimumHeight(size.height() / 2);
  
  QSizePolicy spBase(QSizePolicy::Preferred, QSizePolicy::Preferred);
  spBase.setVerticalStretch(1);
  baseFrame->setSizePolicy(spBase);

  auto layout = new QVBoxLayout();
  layout->setContentsMargins(5, 5, 5, 5);
  layout->addWidget(baseFrame);
  layout->setSizeConstraint(QLayout::SetMinimumSize);

  scrub_layout(this->layout());
  setLayout(layout);
  
  setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void Tile::splitv() {
  auto size = this->frameSize();

  splitState = VERTICAL;
  
  auto panel = (splitTile == nullptr) ? new QFrame() : splitTile;
  panel->setMinimumHeight(size.height() / 2);
  
  QSizePolicy spPanel(QSizePolicy::Preferred, QSizePolicy::Preferred);
  spPanel.setVerticalStretch(1);
  panel->setSizePolicy(spPanel);

  auto vs = new QSplitter(Qt::Vertical, this);
  vs->addWidget(baseFrame);
  vs->addWidget(panel);
  vs->setStretchFactor(1, 1);

  auto layout = new QVBoxLayout();
  layout->setContentsMargins(5, 5, 5, 5);
  layout->addWidget(vs);
  layout->setSizeConstraint(QLayout::SetMinimumSize);

  scrub_layout(this->layout());
  setLayout(layout);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void Tile::splith() {
  auto size = this->frameSize();
  
  splitState = HORIZONTAL;

  auto panel = (splitTile == nullptr) ? new QFrame() : splitTile;
  panel->setMinimumWidth(size.width() / 2);
  
  QSizePolicy spPanel(QSizePolicy::Preferred, QSizePolicy::Preferred);
  spPanel.setHorizontalStretch(1);
  panel->setSizePolicy(spPanel);

  auto hs = new QSplitter(Qt::Horizontal, this);
  hs->addWidget(baseFrame);
  hs->addWidget(panel);
  hs->setStretchFactor(1, 1);

  auto layout = new QVBoxLayout();
  layout->setContentsMargins(5, 5, 5, 5);
  layout->addWidget(hs);
  layout->setSizeConstraint(QLayout::SetMinimumSize);

  scrub_layout(this->layout());
  setLayout(layout);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

Tile::Tile(MainTabBar* tb, QFrame* cf)
  : tabBar(tb), baseFrame(cf) {
  
  splitState = UNSPLIT;
  splitTile = nullptr;

  baseFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
  
  QSizePolicy spBase(QSizePolicy::Preferred, QSizePolicy::Preferred);
  spBase.setVerticalStretch(1);
  baseFrame->setSizePolicy(spBase);

  auto layout = new QVBoxLayout;
  layout->setContentsMargins(5, 5, 5, 5);
  layout->addWidget(baseFrame);

  setLayout(layout);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);  
}

} /* logicaide namespace */
