/* === This file is part of Tomahawk Player - <http://tomahawk-player.org> ===
 *
 *   Copyright 2013, Teo Mrnjavac <teo@kde.org>
 *
 *   Tomahawk is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Tomahawk is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Tomahawk. If not, see <http://www.gnu.org/licenses/>.
 */


#include "ScriptCollection.h"

#include "Source.h"
#include "ExternalResolverGui.h"
#include "utils/TomahawkUtilsGui.h"
#include "utils/Logger.h"
#include "resolvers/ScriptCommand_AllArtists.h"
#include "resolvers/ScriptCommand_AllAlbums.h"
#include "resolvers/ScriptCommand_AllTracks.h"

#include <QIcon>
#include <QPainter>

using namespace Tomahawk;


ScriptCollection::ScriptCollection( const source_ptr& source,
                                    ExternalResolver* resolver,
                                    QObject* parent )
    : Collection( source, QString( "scriptcollection:" + resolver->name() + ":" + uuid() ), parent )
{
    Q_ASSERT( resolver != 0 );
    qDebug() << Q_FUNC_INFO << resolver->name() << name();

    m_resolver = resolver;
}


ScriptCollection::~ScriptCollection()
{

}


QString
ScriptCollection::prettyName() const
{
    if ( source()->isLocal() )
        return tr( "My %1 Collection",
                   "Name of a collection based on a resolver, e.g. My Subsonic Collection" )
                .arg( m_resolver->name() );
    return tr( "%1 Collection of %2",
               "Name of a collection based on a resolver, e.g. Subsonic Collection of Some Dude" )
            .arg( m_resolver->name() )
            .arg( source()->friendlyName() );
}


QString
ScriptCollection::itemName() const
{
    return tr( "%1 Collection",
               "Name of a collection based on a resolver, e.g. Subsonic Collection" )
            .arg( m_resolver->name() );
}


QIcon
ScriptCollection::icon() const
{
    ExternalResolverGui* gResolver = qobject_cast< ExternalResolverGui* >( m_resolver );
    if ( gResolver )
    {
        return gResolver->icon();
    }
    return QIcon();
}


QPixmap
ScriptCollection::bigIcon() const
{
    QPixmap big = Collection::bigIcon();
    QPixmap base = icon().pixmap( big.size() );

    if ( !source()->isLocal() )
    {
        big = big.scaled( TomahawkUtils::defaultIconSize(),
                          Qt::KeepAspectRatio,
                          Qt::SmoothTransformation );

        QPainter painter( &base );
        painter.drawPixmap( base.width() - big.width(),
                            base.height() - big.height(),
                            big.width(),
                            big.height(),
                            big );
        painter.end();
    }

    return base;
}


Tomahawk::ArtistsRequest*
ScriptCollection::requestArtists()
{
    Tomahawk::collection_ptr thisCollection = m_resolver->collections().value( name() );
    if ( thisCollection->name() != this->name() )
        return 0;

    Tomahawk::ArtistsRequest* cmd = new ScriptCommand_AllArtists( thisCollection );

    return cmd;
}


Tomahawk::AlbumsRequest*
ScriptCollection::requestAlbums( const Tomahawk::artist_ptr& artist )
{
    Tomahawk::collection_ptr thisCollection = m_resolver->collections().value( name() );
    if ( thisCollection->name() != this->name() )
        return 0;

    Tomahawk::AlbumsRequest* cmd = new ScriptCommand_AllAlbums( thisCollection, artist );

    return cmd;
}


Tomahawk::TracksRequest*
ScriptCollection::requestTracks( const Tomahawk::album_ptr& album )
{
    Tomahawk::collection_ptr thisCollection = m_resolver->collections().value( name() );
    if ( thisCollection->name() != this->name() )
        return 0;

    Tomahawk::TracksRequest* cmd = new ScriptCommand_AllTracks( thisCollection, album );

    return cmd;
}
