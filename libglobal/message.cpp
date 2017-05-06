/*
 * message.cpp
 * Copyright 2017 - ~, Apin <apin.klas@gmail.com>
 *
 * This file is part of Turbin.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "message.h"
#include "global_constant.h"
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>

using namespace LibG;

Message::Message()
{
}

Message::Message(int type, int command, int status)
{
    setType(type);
    setCommand(command);
    setStatus(status);
}

Message::Message(const QByteArray &ba)
{
    fromByteArray(ba);
}

Message::Message(Message *other)
{
    this->mFlag = other->mFlag;
    this->mSocketId = other->mSocketId;
    this->mUniqueId = other->mUniqueId;
}

void Message::setType(int type)
{
    mFlag &= ~0xFF;
    mFlag |= (type & 0xFF);
}

void Message::setCommand(int command)
{
    mFlag &= ~0xFF00;
    mFlag |= ((command << 8) & 0xFF00);
}

void Message::setStatus(int status)
{
    mFlag &= ~0xF0000;
    mFlag |= ((status << 16) & 0xF0000);
}

void Message::setFlag(int flag)
{
    mFlag &= ~0xFF00000;
    mFlag |= ((flag << 20) & 0xFF00000);
}

void Message::addData(const QString &key, const QVariant &data)
{
    mData.insert(key, data);
}

void Message::clearData()
{
    mData.clear();
}

void Message::setData(const QVariantMap &data)
{
    mData = data;
}

QVariantMap Message::data()
{
    return mData;
}

QVariant Message::data(const QString &key)
{
    return mData[key];
}

void Message::setError(const QString &error)
{
    setStatus(STATUS::ERROR);
    mData.clear();
    mData.insert("error", error);
}

QJsonObject Message::toJsonObject()
{
    QJsonObject root;
    root.insert(QLatin1String("_u"), QJsonValue(mUniqueId));
    root.insert(QLatin1String("_f"), QJsonValue(mFlag));
    root.insert(QLatin1String("_d"), QJsonValue(QJsonObject::fromVariantMap(mData)));
    return root;
}

QString Message::toJsonString()
{
    const QJsonObject &root = toJsonObject();
    QJsonDocument doc(root);
    return QLatin1String(doc.toJson(QJsonDocument::Compact));
}

QByteArray Message::toByteArray()
{
    const QJsonObject &root = toJsonObject();
    QJsonDocument doc(root);
    return qCompress(doc.toBinaryData());
}

void Message::fromByteArray(const QByteArray &ba)
{
    QJsonDocument doc = QJsonDocument::fromBinaryData(qUncompress(ba));
    fromJsonDoc(doc);
}

void Message::fromJsonDoc(const QJsonDocument &jsonDoc)
{
    QJsonObject obj = jsonDoc.object();
    mFlag = obj.value(QLatin1String("_f")).toInt();
    mData = obj.value(QLatin1String("_d")).toObject().toVariantMap();
    mUniqueId = obj.value(QLatin1String("_u")).toInt();
}
