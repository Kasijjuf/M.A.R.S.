/* Freezer.cpp

Copyright (c) 2010 by Felix Lauer and Simon Schneegans

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>. */

# include "Specials/specials.hpp"

# include "SpaceObjects/Ship.hpp"
# include "SpaceObjects/ships.hpp"
# include "SpaceObjects/balls.hpp"
# include "Players/Player.hpp"
# include "System/timer.hpp"
# include "DecoObjects/decoObjects.hpp"

# include <SFML/Graphics.hpp>
# include <vector>

void Freezer::draw() const {
    if (timer_ > 0.f) {
        float alpha(0.f);
        if(timer_ > 0.4f)
            alpha = std::pow(0.5f-timer_,2)*100.f;
        else
            alpha = -2.5f*(0.5f-timer_)+1.25f;

        float scale(1.f-timer_);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 1.0f, alpha);

        const int posX = 3;
        const int posY = 1;

        glBegin(GL_QUADS);
            glTexCoord2f( posX*0.25f,    posY*0.25f);    glVertex2f(-radius_*scale,-radius_*scale);
            glTexCoord2f( posX*0.25f,   (posY+1)*0.25f); glVertex2f(-radius_*scale, radius_*scale);
            glTexCoord2f((posX+1)*0.25f,(posY+1)*0.25f); glVertex2f( radius_*scale, radius_*scale);
            glTexCoord2f((posX+1)*0.25f, posY*0.25f);    glVertex2f( radius_*scale,-radius_*scale);
        glEnd();

        timer_ -= timer::frameTime();
    }
}

void Freezer::activate() const {
    if (parent_->fragStars_ > 0) {
        radius_ = parent_->fragStars_*50.f+25.f;
        std::vector<Ship*> const& ships = ships::getShips();
        for (std::vector<Ship*>::const_iterator it=ships.begin(); it!=ships.end(); ++it) {
            if ((*it)!=parent_ && (*it)->visible_) {
                float distance(((*it)->location_-parent_->location_).length());
                if (distance <= radius_) {
                    (*it)->setDamageSource(parent_->getOwner());
                    (*it)->frozen_=40;
                    (*it)->velocity_=Vector2f();
                    (*it)->mass_=9999999999.f;
                    decoObjects::addShipIce(*it);
                }
            }
        }
        Ball* ball = balls::getBall();

        if(ball && ball->visible_) {
            float distance((ball->location_-parent_->location_).length());
                if (distance <= radius_) {
                    ball->frozen_=40;
                    ball->velocity_=Vector2f();
                    ball->mass_=9999999999.f;
                    decoObjects::addBallIce(ball);
            }
        }
        timer_ = 0.5f;
        parent_->fragStars_ = 0;
    }
}

void Freezer::next() {
    parent_->currentSpecial_ = new Blast(parent_);
    delete this;
}

void Freezer::previous() {
    parent_->currentSpecial_ = new Heal(parent_);
    delete this;
}


