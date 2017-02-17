#ifndef _AGVPARA_C
#define _AGVPARA_C
#include "agvparastruct.h"
#include <QtSql>

//########################################################################################
// Function Name:	agvpara
// Version:			V1.0
// Input:
// Output:
// Description:
//########################################################################################
//Tag_backAgvState tagBackAgvState;
//Tag_backPosition tagBackPosition;
//Tag_backSpdAndSensor tagBackSpdAndSensor;
//QList<Tag_Nposition> m_tagNormalAimposition;
//======================================================
//初始化AGV参数函数
//======================================================
void agvMapPara_Init()
{
    uAlreadySendAGVPositionCnt = 0;
    tagAgvBackAgvState.field.state = 0;
    tagAgvBackPosition.field.A.d = 23;
    tagAgvBackPosition.field.w.d = 34;
    tagAgvBackPosition.field.x.d = 78;
    tagAgvBackPosition.field.y.d = 66;
    tagAgvBackSpdAndSensor.field.forwardSpd.d = 0;
    tagAgvNormalAimposition.clear();

}
//======================================================
//解析AGV状态函数
//======================================================
void agvMapPara_Statehandle()
{
    switch (tagAgvBackAgvState.field.state) {
    case 0:
    case 1:
        ushortAgvState = StartExecution;
        break;
    case 3:
        ushortAgvState = SuccessArrival;
        break;
    case 4:
        ushortAgvState = FailedArrival;
        break;
    case 2:
    case 5:
    case 6:
    case 7:
    case 8:
        ushortAgvState = MissionCancle;
        break;
    case 9:
        ushortAgvState = LostAim;
        break;
    default:
        break;
    }
}




bool  dbSetMapUrl(QString URL)
{
   QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
   db.setDatabaseName("robot");
   if(!db.open())
   {
       qDebug()<<db.lastError();
       qFatal("failed to connect.") ;
        db.close();
       return false;
   }
   else
   {
       //QSqlQuery类提供执行和操作的SQL语句的方法。
       //可以用来执行DML（数据操作语言）语句，如SELECT、INSERT、UPDATE、DELETE，
       //以及DDL（数据定义语言）语句，例如CREATE TABLE。
       //也可以用来执行那些不是标准的SQL的数据库特定的命令。
       QSqlQuery sql_query;
       QString create_sql = "CREATE TABLE IF NOT EXISTS ImageUrl (id INTEGER PRIMARY KEY, name varchar(150))";
       QString select_max_sql = "select max(id) from ImageUrl";
       QString insert_sql = "insert into ImageUrl values (?, ?)";
       QString update_sql = "update ImageUrl set name = :name where id = :id";
       QString select_sql = "select id, name from student";
       QString select_all_sql = "select * from ImageUrl";
       QString delete_sql = "delete from ImageUrl where id = ?";
       QString clear_sql = "delete from ImageUrl";
       sql_query.prepare(create_sql);
       if(!sql_query.exec())
       {
            qDebug()<<sql_query.lastError();
             db.close();
            return false;
       }
       else
       {
            qDebug()<<"table created!";
       }
       //查询最大id
       int max_id = 0;
       sql_query.prepare(select_max_sql);
       if(!sql_query.exec())
       {
            qDebug()<<sql_query.lastError();
       }
       else
       {
           while(sql_query.next())
           {
               max_id = sql_query.value(0).toInt();
               qDebug()<<QString("max id:%1").arg(max_id);
           }
       }
       if(max_id == 0) //如果是一张空表
       {
            //insert
           //插入数据
           sql_query.prepare(insert_sql);
           sql_query.addBindValue(max_id+1);
           sql_query.addBindValue(URL);
           if(!sql_query.exec())
           {
                qDebug()<<sql_query.lastError();
           }
           else
           {
                qDebug()<<"inserted!";
           }

       }
       else
       {
           //update
           //更新数据
           sql_query.prepare(update_sql);
           sql_query.bindValue(":name", URL);
           sql_query.bindValue(":id", 1);
           if(!sql_query.exec())
           {
           qDebug()<<sql_query.lastError();
             db.close();
            return false;
           }
           else
           {
           qDebug()<<"updated!";
             db.close();
             return true;
           }
       }
   }

}

QString dbReadMapUrl()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("robot");
    if(!db.open())
    {
        qDebug()<<db.lastError();
        qFatal("failed to connect.") ;
        return false;
    }
    else
    {
        //QSqlQuery类提供执行和操作的SQL语句的方法。
        //可以用来执行DML（数据操作语言）语句，如SELECT、INSERT、UPDATE、DELETE，
        //以及DDL（数据定义语言）语句，例如CREATE TABLE。
        //也可以用来执行那些不是标准的SQL的数据库特定的命令。
        QSqlQuery sql_query;
        QString create_sql = "CREATE TABLE IF NOT EXISTS ImageUrl (id INTEGER PRIMARY KEY, name varchar(150))";
        QString select_max_sql = "select max(id) from ImageUrl";
        QString insert_sql = "insert into ImageUrl values (?, ?)";
        QString update_sql = "update ImageUrl set name = :name where id = :id";
        QString select_sql = "select id, name from ImageUrl";
        QString select_all_sql = "select * from ImageUrl";
        QString delete_sql = "delete from ImageUrl where id = ?";
        QString clear_sql = "delete from ImageUrl";
        //查询最大id
        int max_id = 0;
        sql_query.prepare(select_max_sql);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
            return 0;
        }
        else
        {
            while(sql_query.next())
            {
                max_id = sql_query.value(0).toInt();
                qDebug()<<QString("max id:%1").arg(max_id);
            }
        }
        //查询部分数据
        //        sql_query.prepare(update_sql);
        //        sql_query.bindValue(":name", "hyman");
        //        sql_query.bindValue(":id", 1);
        if(max_id > 0)
        {
            if(!sql_query.exec(select_sql))
            {
                qDebug()<<sql_query.lastError();
            }
            else
            {
                while(sql_query.next())
                {
                int id = sql_query.value("id").toInt();
                QString name = sql_query.value("name").toString();

                qDebug()<<QString("id:%1    name:%2").arg(id).arg(name);
                return name;

                }

            }
        }
        else
        {
            return 0;
        }

    }
}

bool dbSetOffset(float xoffset,float yoffset)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("robot");
    if(!db.open())
    {
        qDebug()<<db.lastError();
        qFatal("failed to connect.") ;
         db.close();
        return false;
    }
    else
    {
        //QSqlQuery类提供执行和操作的SQL语句的方法。
        //可以用来执行DML（数据操作语言）语句，如SELECT、INSERT、UPDATE、DELETE，
        //以及DDL（数据定义语言）语句，例如CREATE TABLE。
        //也可以用来执行那些不是标准的SQL的数据库特定的命令。
        QSqlQuery sql_query;
        QString create_sql = "CREATE TABLE IF NOT EXISTS MapOffset (id INTEGER PRIMARY KEY,offsetx INTEGER,offsety INTEGER)";
        QString select_max_sql = "select max(id) from MapOffset";
        QString insert_sql = "insert into MapOffset values (?, ?, ?)";
        QString update_sql = "update MapOffset set offsetx = :offsetx, offsety = :offsety where id = :id";
        QString select_sql = "select id, offsetx,offsety from MapOffset";
        QString select_all_sql = "select * from MapOffset";
        QString delete_sql = "delete from MapOffset where id = ?";
        QString clear_sql = "delete from MapOffset";
        sql_query.prepare(create_sql);
        if(!sql_query.exec())
        {
             qDebug()<<sql_query.lastError();
              db.close();
             return false;
        }
        else
        {
             qDebug()<<"table created!";
        }
        //查询最大id
        int max_id = 0;
        sql_query.prepare(select_max_sql);
        if(!sql_query.exec())
        {
             qDebug()<<sql_query.lastError();
        }
        else
        {
            while(sql_query.next())
            {
                max_id = sql_query.value(0).toInt();
                qDebug()<<QString("max id:%1").arg(max_id);
            }
        }
        if(max_id == 0) //如果是一张空表
        {
             //insert
            //插入数据
            sql_query.prepare(insert_sql);
            sql_query.addBindValue(max_id+1);
            sql_query.addBindValue(xoffset);
            sql_query.addBindValue(yoffset);
            if(!sql_query.exec())
            {
                 qDebug()<<sql_query.lastError();
            }
            else
            {
                 qDebug()<<"inserted!";
            }

        }
        else
        {
            //update
            //更新数据
            sql_query.prepare(update_sql);
            sql_query.bindValue(":id", 1);
            sql_query.bindValue(":offsetx", xoffset);
            sql_query.bindValue(":offsety", yoffset);
            if(!sql_query.exec())
            {
            qDebug()<<sql_query.lastError();
              db.close();
             return false;
            }
            else
            {
            qDebug()<<"updated!";
              db.close();
              return true;
            }
        }
    }
}

float dbReadXoffset()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("robot");
    if(!db.open())
    {
        qDebug()<<db.lastError();
        qFatal("failed to connect.") ;
        db.close();
        return false;
    }
    else
    {
        //QSqlQuery类提供执行和操作的SQL语句的方法。
        //可以用来执行DML（数据操作语言）语句，如SELECT、INSERT、UPDATE、DELETE，
        //以及DDL（数据定义语言）语句，例如CREATE TABLE。
        //也可以用来执行那些不是标准的SQL的数据库特定的命令。
        QSqlQuery sql_query;
        QString create_sql = "CREATE TABLE IF NOT EXISTS MapOffset (id INTEGER PRIMARY KEY,offsetx INTEGER,offsety INTEGER)";
        QString select_max_sql = "select max(id) from MapOffset";
        QString insert_sql = "insert into MapOffset values (?, ?, ?)";
        QString update_sql = "update MapOffset set offsetx = :offsetx, offsety = :offsety where id = :id";
        QString select_sql = "select id, offsetx,offsety from MapOffset";
        QString select_all_sql = "select * from MapOffset";
        QString delete_sql = "delete from MapOffset where id = ?";
        QString clear_sql = "delete from MapOffset";
        //查询最大id
        int max_id = 0;
        sql_query.prepare(select_max_sql);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
            db.close();
            return 0;
        }
        else
        {
            while(sql_query.next())
            {
                max_id = sql_query.value(0).toInt();
                qDebug()<<QString("max id:%1").arg(max_id);
            }
        }
        //查询部分数据
        //        sql_query.prepare(update_sql);
        //        sql_query.bindValue(":name", "hyman");
        //        sql_query.bindValue(":id", 1);
        if(max_id > 0)
        {
            if(!sql_query.exec(select_sql))
            {
                qDebug()<<sql_query.lastError();
            }
            else
            {
                while(sql_query.next())
                {
                int id = sql_query.value("id").toInt();
                float offsetx = sql_query.value("offsetx").toFloat();

                qDebug()<<QString("id:%1    offsetx:%2").arg(id).arg(offsetx);
                db.close();
                return offsetx;

                }

            }
        }
        else
        {
            return 0;
        }

    }
}

float dbReadYoffset()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("robot");
    if(!db.open())
    {
        qDebug()<<db.lastError();
        qFatal("failed to connect.");
        db.close();
        return false;
    }
    else
    {
        //QSqlQuery类提供执行和操作的SQL语句的方法。
        //可以用来执行DML（数据操作语言）语句，如SELECT、INSERT、UPDATE、DELETE，
        //以及DDL（数据定义语言）语句，例如CREATE TABLE。
        //也可以用来执行那些不是标准的SQL的数据库特定的命令。
        QSqlQuery sql_query;
        QString create_sql = "CREATE TABLE IF NOT EXISTS MapOffset (id INTEGER PRIMARY KEY,offsetx INTEGER,offsety INTEGER)";
        QString select_max_sql = "select max(id) from MapOffset";
        QString insert_sql = "insert into MapOffset values (?, ?, ?)";
        QString update_sql = "update MapOffset set offsetx = :offsetx, offsety = :offsety where id = :id";
        QString select_sql = "select id, offsetx,offsety from MapOffset";
        QString select_all_sql = "select * from MapOffset";
        QString delete_sql = "delete from MapOffset where id = ?";
        QString clear_sql = "delete from MapOffset";
        //查询最大id
        int max_id = 0;
        sql_query.prepare(select_max_sql);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
            db.close();
            return 0;
        }
        else
        {
            while(sql_query.next())
            {
                max_id = sql_query.value(0).toInt();
                qDebug()<<QString("max id:%1").arg(max_id);
            }
        }
        //查询部分数据
        //        sql_query.prepare(update_sql);
        //        sql_query.bindValue(":name", "hyman");
        //        sql_query.bindValue(":id", 1);
        if(max_id > 0)
        {
            if(!sql_query.exec(select_sql))
            {
                qDebug()<<sql_query.lastError();
            }
            else
            {
                while(sql_query.next())
                {
                int id = sql_query.value("id").toInt();
                float offsetx = sql_query.value("offsety").toFloat();

                qDebug()<<QString("id:%1    offsety:%2").arg(id).arg(offsetx);
                db.close();
                return offsetx;

                }
            }
        }
        else
        {
            return 0;
        }

    }
}

//==============================================================================
//在数据库中写标签信息 X坐标，Y坐标，旋转角度，X坐标比例，Y坐标比例（坐标是通过自动计算方式）
//==============================================================================
bool dbSetMapLabel(QString name,int x,int y,float Rotation,int xScale,int yScale)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("robot");
    if(!db.open())
    {
        qDebug()<<db.lastError();
        qFatal("failed to connect.") ;
         db.close();
        return false;
    }
    else
    {
        //QSqlQuery类提供执行和操作的SQL语句的方法。
        //可以用来执行DML（数据操作语言）语句，如SELECT、INSERT、UPDATE、DELETE，
        //以及DDL（数据定义语言）语句，例如CREATE TABLE。
        //也可以用来执行那些不是标准的SQL的数据库特定的命令。
        QSqlQuery sql_query;
        QString create_sql = "CREATE TABLE IF NOT EXISTS MapLabel (id INTEGER PRIMARY KEY,name varchar(50),xPix INTEGER"
                             ",yPix INTEGER,rotation FLOAT,xScale INTEGER,yScale INTEGER)";
        QString select_max_sql = "select max(id) from MapLabel";
        QString insert_sql = "insert into MapLabel values (?, ?, ?, ?, ?, ?, ?)";
        QString update_sql = "update MapLabel set name = :name, xPix = :xPix, yPix = :yPix,  rotation = :rotation, xScale = :xScale"
                             "yScale = ：yScale where id = :id";
        QString select_sql = "select id, name, xPix, yPix, rotation, xScale, yScale from MapLabel";
        QString select_all_sql = "select * from MapLabel";
        QString delete_sql = "delete from MapLabel where id = ?";
        QString clear_sql = "delete from MapLabel";
        sql_query.prepare(create_sql);
        if(!sql_query.exec())
        {
             qDebug()<<sql_query.lastError();
              db.close();
             return false;
        }
        else
        {
             qDebug()<<"table created!";
        }
        //查询最大id
        int max_id = 0;
        sql_query.prepare(select_max_sql);
        if(!sql_query.exec())
        {
             qDebug()<<sql_query.lastError();
        }
        else
        {
            while(sql_query.next())
            {
                max_id = sql_query.value(0).toInt();
                qDebug()<<QString("max id:%1").arg(max_id);
            }
        }

         //insert
        //插入数据
        sql_query.prepare(insert_sql);
        sql_query.addBindValue(max_id+1);
        sql_query.addBindValue(name);
        sql_query.addBindValue(x);
        sql_query.addBindValue(y);
        sql_query.addBindValue(Rotation);
        sql_query.addBindValue(xScale);
        sql_query.addBindValue(yScale);
        if(!sql_query.exec())
        {
             qDebug()<<sql_query.lastError();
        }
        else
        {
             qDebug()<<"inserted!";
        }
    }
}
//==============================================================================
//在数据库中写标签
//==============================================================================
int dbReadMapLabelMax()
{

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("robot");
    if(!db.open())
    {
        qDebug()<<db.lastError();
        qFatal("failed to connect.") ;
        db.close();
        return 0;
    }
    else
    {
        //QSqlQuery类提供执行和操作的SQL语句的方法。
        //可以用来执行DML（数据操作语言）语句，如SELECT、INSERT、UPDATE、DELETE，
        //以及DDL（数据定义语言）语句，例如CREATE TABLE。
        //也可以用来执行那些不是标准的SQL的数据库特定的命令。
        QSqlQuery sql_query;
        QString select_max_sql = "select max(id) from MapLabel";

        //查询最大id
        int max_id = 0;
        sql_query.prepare(select_max_sql);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
            db.close();
            return 0;
        }
        else
        {
            while(sql_query.next())
            {
                max_id = sql_query.value(0).toInt();
                qDebug()<<QString("max id:%1").arg(max_id);
            }
        }
        return max_id;
    }

}
//==============================================================================
//在数据库中写标签
//==============================================================================
Str_Label dbReadMapLabel(int id)
{
    Str_Label strMapLabel;
    strMapLabel.name = "";
    strMapLabel.x = 0;
    strMapLabel.y = 0;
    strMapLabel.Rotation = 0;
    strMapLabel.xScale = 0;
    strMapLabel.yScale = 0;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("robot");
    if(!db.open())
    {
        qDebug()<<db.lastError();
        qFatal("failed to connect.") ;
        db.close();
        return strMapLabel;
    }
    else
    {
        //QSqlQuery类提供执行和操作的SQL语句的方法。
        //可以用来执行DML（数据操作语言）语句，如SELECT、INSERT、UPDATE、DELETE，
        //以及DDL（数据定义语言）语句，例如CREATE TABLE。
        //也可以用来执行那些不是标准的SQL的数据库特定的命令。
        QSqlQuery sql_query;
        QString create_sql = "CREATE TABLE IF NOT EXISTS MapLabel (id INTEGER PRIMARY KEY,name varchar(50),xPix INTEGER"
                             ",yPix INTEGER,rotation FLOAT,xScale INTEGER,yScale INTEGER)";
        QString select_max_sql = "select max(id) from MapLabel";
        QString insert_sql = "insert into MapLabel values (?, ?, ?, ?, ?, ?, ?)";
        QString update_sql = "update MapLabel set name = :name, xPix = :xPix, yPix = :yPix,  rotation = :rotation, xScale = :xScale"
                             "yScale = ：yScale where id = :id";
        QString select_sql = "select id, name, xPix, yPix, rotation, xScale, yScale from MapLabel";
        QString select_sql_id = "select * from MapLabel where id = ?";
        QString select_all_sql = "select * from MapLabel";
        QString delete_sql = "delete from MapLabel where id = ?";
        QString clear_sql = "delete from MapLabel";
        //查询最大id
        int max_id = 0;
        sql_query.prepare(select_max_sql);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
            db.close();
            return strMapLabel;
        }
        else
        {
            while(sql_query.next())
            {
                max_id = sql_query.value(0).toInt();
                qDebug()<<QString("max id:%1").arg(max_id);
            }
        }
        //查询部分数据
        //        sql_query.prepare(update_sql);
        //        sql_query.bindValue(":name", "hyman");
        //        sql_query.bindValue(":id", 1);
        if(max_id > 0)
        {
//            select_sql.arg(1);
            sql_query.prepare(select_sql_id);
            sql_query.addBindValue(id);

            if(!sql_query.exec())
            {
                qDebug()<<sql_query.lastError();
            }
            else
            {
//                while(sql_query.next())
//                {
                sql_query.next(); //不是很理解为什么需要.next
                int id = sql_query.value("id").toInt();
                strMapLabel.name = sql_query.value("name").toString();
                strMapLabel.x = sql_query.value("xPix").toInt();
                strMapLabel.y = sql_query.value("yPix").toInt();
                strMapLabel.Rotation = sql_query.value("rotation").toFloat();
                strMapLabel.xScale = sql_query.value("xScale").toFloat();
                strMapLabel.yScale = sql_query.value("yScale").toFloat();

                qDebug()<<QString("id:%1 name:%2 xPix:%3").arg(id).arg( strMapLabel.name).arg(strMapLabel.x);
                db.close();
                return strMapLabel;

//                }

            }
        }
        else
        {
            return strMapLabel;
        }

    }
}
//==============================================================================
//清楚标签数据表
//==============================================================================
bool dbLabelDeleteAll()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("robot");
    if(!db.open())
    {
        qDebug()<<db.lastError();
        qFatal("failed to connect.") ;
        db.close();
        return false;
    }
    else
    {
        //QSqlQuery类提供执行和操作的SQL语句的方法。
        //可以用来执行DML（数据操作语言）语句，如SELECT、INSERT、UPDATE、DELETE，
        //以及DDL（数据定义语言）语句，例如CREATE TABLE。
        //也可以用来执行那些不是标准的SQL的数据库特定的命令。
        QSqlQuery sql_query;
        QString clear_sql = "delete from MapLabel";
        //清空表
        sql_query.prepare(clear_sql);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
            db.close();
            return false;
        }
        else
        {
            return true;
        }
  }
}

bool dbLabelDeleteOne(int x,int y)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("robot");
    if(!db.open())
    {
        qDebug()<<db.lastError();
        qFatal("failed to connect.") ;
        db.close();
        return false;
    }
    else
    {
        //QSqlQuery类提供执行和操作的SQL语句的方法。
        //可以用来执行DML（数据操作语言）语句，如SELECT、INSERT、UPDATE、DELETE，
        //以及DDL（数据定义语言）语句，例如CREATE TABLE。
        //也可以用来执行那些不是标准的SQL的数据库特定的命令。
        QSqlQuery sql_query;
        QString clear_sql_one = "delete from MapLabel where xPix = ? and yPix= ?";
        //清空表
        sql_query.prepare(clear_sql_one);
        sql_query.addBindValue(x);
        sql_query.addBindValue(y);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
            db.close();
            return false;
        }
        else
        {
            return true;
        }
  }
}
bool readLabePosition() //读标签到缓存
{
    int imax;
    Str_Label strLabel;
    tagMapLabelList.clear();
    imax = dbReadMapLabelMax();
    for(int i = 0; i < imax ; i++)
    {
        strLabel = dbReadMapLabel(i + 1);
        tagMapLabelList.append(strLabel);
    }
    return true;
}
//=====================================================
//保存初始位置
//=====================================================
bool dbSetStartPosition(float x,float y,float w,float a)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("robot");
    if(!db.open())
    {
        qDebug()<<db.lastError();
        qFatal("failed to connect.") ;
         db.close();
        return false;
    }
    else
    {
        //QSqlQuery类提供执行和操作的SQL语句的方法。
        //可以用来执行DML（数据操作语言）语句，如SELECT、INSERT、UPDATE、DELETE，
        //以及DDL（数据定义语言）语句，例如CREATE TABLE。
        //也可以用来执行那些不是标准的SQL的数据库特定的命令。
        QSqlQuery sql_query;
        QString create_sql = "CREATE TABLE IF NOT EXISTS StartPosition (id INTEGER PRIMARY KEY,x FLOAT,y FLOAT,w FLOAT,a FlOAT)";
        QString select_max_sql = "select max(id) from StartPosition";
        QString insert_sql = "insert into StartPosition values (?, ?, ?, ?, ?)";
        QString update_sql = "update StartPosition set x = :x, y = :y,w = :w,a = :a where id = :id";
        QString select_sql = "select id, x, y, w, a from StartPosition";
        QString select_all_sql = "select * from StartPosition";
        QString delete_sql = "delete from StartPosition where id = ?";
        QString clear_sql = "delete from StartPosition";
        sql_query.prepare(create_sql);
        if(!sql_query.exec())
        {
             qDebug()<<sql_query.lastError();
              db.close();
             return false;
        }
        else
        {
             qDebug()<<"table created!";
        }
        //查询最大id
        int max_id = 0;
        sql_query.prepare(select_max_sql);
        if(!sql_query.exec())
        {
             qDebug()<<sql_query.lastError();
        }
        else
        {
            while(sql_query.next())
            {
                max_id = sql_query.value(0).toInt();
                qDebug()<<QString("max id:%1").arg(max_id);
            }
        }
        if(max_id == 0) //如果是一张空表
        {
             //insert
            //插入数据
            sql_query.prepare(insert_sql);
            sql_query.addBindValue(max_id+1);
            sql_query.addBindValue(x);
            sql_query.addBindValue(y);
            sql_query.addBindValue(w);
            sql_query.addBindValue(a);
            if(!sql_query.exec())
            {
                 qDebug()<<sql_query.lastError();
            }
            else
            {
                 qDebug()<<"inserted!";
            }

        }
        else
        {
            //update
            //更新数据
            sql_query.prepare(update_sql);
            sql_query.bindValue(":id", 1);
            sql_query.bindValue(":x", x);
            sql_query.bindValue(":y", y);
            sql_query.bindValue(":w", w);
            sql_query.bindValue(":a", a);
            if(!sql_query.exec())
            {
            qDebug()<<sql_query.lastError();
              db.close();
             return false;
            }
            else
            {
            qDebug()<<"updated!";
              db.close();
              return true;
            }
        }
    }
}

Tag_position dbReadStartPosition()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("robot");
    Tag_position tagPosition;
    tagPosition.filed.x.d = 0;
    tagPosition.filed.y.d = 0;
    tagPosition.filed.A.d = 0;
    tagPosition.filed.w.d = 0;
    if(!db.open())
    {
        qDebug()<<db.lastError();
        qFatal("failed to connect.") ;
        db.close();
        return tagPosition;
    }
    else
    {
        //QSqlQuery类提供执行和操作的SQL语句的方法。
        //可以用来执行DML（数据操作语言）语句，如SELECT、INSERT、UPDATE、DELETE，
        //以及DDL（数据定义语言）语句，例如CREATE TABLE。
        //也可以用来执行那些不是标准的SQL的数据库特定的命令。
        QSqlQuery sql_query;
        QString create_sql = "CREATE TABLE IF NOT EXISTS StartPosition (id INTEGER PRIMARY KEY,x FLOAT,y FLOAT,w FLOAT,a FlOAT)";
        QString select_max_sql = "select max(id) from StartPosition";
        QString insert_sql = "insert into StartPosition values (?, ?, ?, ?, ?)";
        QString update_sql = "update StartPosition set x = :x, y = :y,w = :w,a = ：a where id = :id";
        QString select_sql = "select id, x,y,w,a from StartPosition";
        QString select_all_sql = "select * from StartPosition";
        QString delete_sql = "delete from StartPosition where id = ?";
        QString clear_sql = "delete from StartPosition";
        //查询最大id
        int max_id = 0;
        sql_query.prepare(select_max_sql);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
            db.close();
            return tagPosition;
        }
        else
        {
            while(sql_query.next())
            {
                max_id = sql_query.value(0).toInt();
                qDebug()<<QString("max id:%1").arg(max_id);
            }
        }
        //查询部分数据
        //        sql_query.prepare(update_sql);
        //        sql_query.bindValue(":name", "hyman");
        //        sql_query.bindValue(":id", 1);
        if(max_id > 0)
        {
            if(!sql_query.exec(select_sql))
            {
                qDebug()<<sql_query.lastError();
            }
            else
            {
                while(sql_query.next())
                {
                int id = sql_query.value("id").toInt();

                tagPosition.filed.x.d = sql_query.value("x").toFloat();
                tagPosition.filed.y.d = sql_query.value("y").toFloat();
                tagPosition.filed.w.d = sql_query.value("w").toFloat();
                tagPosition.filed.A.d = sql_query.value("a").toFloat();
                qDebug()<<QString("id:%1    x:%2").arg(id).arg(tagPosition.filed.x.d);
                db.close();
                return tagPosition;

                }

            }
        }
        else
        {
            return tagPosition;
        }

    }
}

void readStartPosition()
{
     tagStartPosition = dbReadStartPosition();
}

//=============================================================
//固定标签操作--四元数坐标是通过人工输入的方式建立的标签
//=============================================================
bool dbSetfixMapLabel(QString name,int iPicX,int iPicY,float fMapx,float fMapy,float omega,float theta,
                            int width,int height)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("robot");
    if(!db.open())
    {
        qDebug()<<db.lastError();
        qFatal("failed to connect.") ;
         db.close();
        return false;
    }
    else
    {
        //QSqlQuery类提供执行和操作的SQL语句的方法。
        //可以用来执行DML（数据操作语言）语句，如SELECT、INSERT、UPDATE、DELETE，
        //以及DDL（数据定义语言）语句，例如CREATE TABLE。
        //也可以用来执行那些不是标准的SQL的数据库特定的命令。
        QSqlQuery sql_query;
        QString create_sql = "CREATE TABLE IF NOT EXISTS FixMapLabel (id INTEGER PRIMARY KEY,name varchar(50),xPix INTEGER"
                             ",yPix INTEGER,xMap FLOAT,yMap FLOAT,omega FLOAT,theta FLOAT,height INTEGER,width INTEGER)";
        QString select_max_sql = "select max(id) from FixMapLabel";
        QString insert_sql = "insert into FixMapLabel values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
        QString update_sql = "update FixMapLabel set name = :name, xPix = :xPix, yPix = :yPix, xMap = :xMap, yMap = :yMap, omega = :omega,"
                             "theta = :theta ,height = :height,width = :width where id = :id";
        QString select_sql = "select id, name, xPix, yPix, xMap, yMap, omega, theta, height, width from FixMapLabel";
        QString select_all_sql = "select * from FixMapLabel";
        QString delete_sql = "delete from FixMapLabel where id = ?";
        QString clear_sql = "delete from FixMapLabel";
        sql_query.prepare(create_sql);
        if(!sql_query.exec())
        {
             qDebug()<<sql_query.lastError();
              db.close();
             return false;
        }
        else
        {
             qDebug()<<"table created!";
        }
        //查询最大id
        int max_id = 0;
        sql_query.prepare(select_max_sql);
        if(!sql_query.exec())
        {
             qDebug()<<sql_query.lastError();
        }
        else
        {
            while(sql_query.next())
            {
                max_id = sql_query.value(0).toInt();
                qDebug()<<QString("max id:%1").arg(max_id);
            }
        }

         //insert
        //插入数据
        sql_query.prepare(insert_sql);
        sql_query.addBindValue(max_id+1);
        sql_query.addBindValue(name);
        sql_query.addBindValue(iPicX);
        sql_query.addBindValue(iPicY);
        sql_query.addBindValue(fMapx);
        sql_query.addBindValue(fMapy);
        sql_query.addBindValue(omega);
        sql_query.addBindValue(theta);
        sql_query.addBindValue(height);
        sql_query.addBindValue(width);
        if(!sql_query.exec())
        {
             qDebug()<<sql_query.lastError();
        }
        else
        {
             qDebug()<<"inserted!";
        }
    }
}

bool dbFixLabelDeleteAll()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("robot");
    if(!db.open())
    {
        qDebug()<<db.lastError();
        qFatal("failed to connect.") ;
        db.close();
        return false;
    }
    else
    {
        //QSqlQuery类提供执行和操作的SQL语句的方法。
        //可以用来执行DML（数据操作语言）语句，如SELECT、INSERT、UPDATE、DELETE，
        //以及DDL（数据定义语言）语句，例如CREATE TABLE。
        //也可以用来执行那些不是标准的SQL的数据库特定的命令。
        QSqlQuery sql_query;
        QString clear_sql = "delete from FixMapLabel";
        //清空表
        sql_query.prepare(clear_sql);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
            db.close();
            return false;
        }
        else
        {
            return true;
        }
  }
}

Str_FixLabel dbReadFixMapLabel(int id)
{
    Str_FixLabel strFixMapLabel;
    strFixMapLabel.name = "";
    strFixMapLabel.iPicX = 0;
    strFixMapLabel.iPicY = 0;
    strFixMapLabel.fMapX = 0;
    strFixMapLabel.fMapY = 0;
    strFixMapLabel.fMapTheta = 0;
    strFixMapLabel.fMapOmega = 0;
    strFixMapLabel.ilabelWidth = 0;
    strFixMapLabel.ilabelHeight = 0;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("robot");
    if(!db.open())
    {
        qDebug()<<db.lastError();
        qFatal("failed to connect.") ;
        db.close();
        return strFixMapLabel;
    }
    else
    {
        //QSqlQuery类提供执行和操作的SQL语句的方法。
        //可以用来执行DML（数据操作语言）语句，如SELECT、INSERT、UPDATE、DELETE，
        //以及DDL（数据定义语言）语句，例如CREATE TABLE。
        //也可以用来执行那些不是标准的SQL的数据库特定的命令。
        QSqlQuery sql_query;
        QString create_sql = "CREATE TABLE IF NOT EXISTS FixMapLabel (id INTEGER PRIMARY KEY,name varchar(50),xPix INTEGER"
                             ",yPix INTEGER,xMap FLOAT,yMap FLOAT,omega FLOAT,theta FLOAT,height INTEGER,width INTEGER)";
        QString select_max_sql = "select max(id) from FixMapLabel";
        QString insert_sql = "insert into FixMapLabel values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
        QString update_sql = "update FixMapLabel set name = :name, xPix = :xPix, yPix = :yPix, xMap = :xMap, yMap = :yMap, omega = :omega,"
                             "theta = :theta ,height = :height,width = :width where id = :id";
        QString select_sql = "select id, name, xPix, yPix, xMap, yMap, omega, theta, height, width from FixMapLabel";
        QString select_all_sql = "select * from FixMapLabel";
        QString select_sql_id = "select * from FixMapLabel where id = ?";
        QString delete_sql = "delete from FixMapLabel where id = ?";
        QString clear_sql = "delete from FixMapLabel";
        //查询最大id
        int max_id = 0;
        sql_query.prepare(select_max_sql);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
            db.close();
            return strFixMapLabel;
        }
        else
        {
            while(sql_query.next())
            {
                max_id = sql_query.value(0).toInt();
                qDebug()<<QString("max id:%1").arg(max_id);
            }
        }
        //查询部分数据
        //        sql_query.prepare(update_sql);
        //        sql_query.bindValue(":name", "hyman");
        //        sql_query.bindValue(":id", 1);
        if(max_id > 0)
        {
//            select_sql.arg(1);
            sql_query.prepare(select_sql_id);
            sql_query.addBindValue(id);

            if(!sql_query.exec())
            {
                qDebug()<<sql_query.lastError();
            }
            else
            {
//                while(sql_query.next())
//                {
                sql_query.next(); //不是很理解为什么需要.next
                int id = sql_query.value("id").toInt();
                strFixMapLabel.name = sql_query.value("name").toString();
                strFixMapLabel.iPicX = sql_query.value("xPix").toInt();
                strFixMapLabel.iPicY = sql_query.value("yPix").toInt();
                strFixMapLabel.fMapX = sql_query.value("xMap").toFloat();
                strFixMapLabel.fMapY = sql_query.value("yMap").toFloat();
                strFixMapLabel.fMapOmega = sql_query.value("omega").toInt();
                strFixMapLabel.fMapTheta = sql_query.value("theta").toInt();
                strFixMapLabel.ilabelWidth = sql_query.value("width").toInt();
                strFixMapLabel.ilabelHeight = sql_query.value("height").toInt();

                qDebug()<<QString("id:%1 name:%2 xPix:%3").arg(id).arg( strFixMapLabel.name).arg(strFixMapLabel.iPicX);
                db.close();
                return strFixMapLabel;
            }
        }
        else
        {
            return strFixMapLabel;
        }
    }
}
bool readFixLabelPosition()
{
    int imax;
    Str_FixLabel strFixLabel;
    tagFixMapLabelList.clear();
    imax = dbReadFixMapLabelMax();
    for(int i = 0; i < imax ; i++)
    {
        strFixLabel = dbReadFixMapLabel(i + 1);
        tagFixMapLabelList.append(strFixLabel);
    }
    return true;
}

int dbReadFixMapLabelMax()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("robot");
    if(!db.open())
    {
        qDebug()<<db.lastError();
        qFatal("failed to connect.") ;
        db.close();
        return 0;
    }
    else
    {
        //QSqlQuery类提供执行和操作的SQL语句的方法。
        //可以用来执行DML（数据操作语言）语句，如SELECT、INSERT、UPDATE、DELETE，
        //以及DDL（数据定义语言）语句，例如CREATE TABLE。
        //也可以用来执行那些不是标准的SQL的数据库特定的命令。
        QSqlQuery sql_query;
        QString select_max_sql = "select max(id) from FixMapLabel";

        //查询最大id
        int max_id = 0;
        sql_query.prepare(select_max_sql);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
            db.close();
            return 0;
        }
        else
        {
            while(sql_query.next())
            {
                max_id = sql_query.value(0).toInt();
                qDebug()<<QString("max id:%1").arg(max_id);
            }
        }
        return max_id;
    }
}
//================================================
//巡检标签
//================================================
bool dbPollingLabelDeleteAll()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("robot");
    if(!db.open())
    {
        qDebug()<<db.lastError();
        qFatal("failed to connect.") ;
        db.close();
        return false;
    }
    else
    {
        //QSqlQuery类提供执行和操作的SQL语句的方法。
        //可以用来执行DML（数据操作语言）语句，如SELECT、INSERT、UPDATE、DELETE，
        //以及DDL（数据定义语言）语句，例如CREATE TABLE。
        //也可以用来执行那些不是标准的SQL的数据库特定的命令。
        QSqlQuery sql_query;
        QString clear_sql = "delete from PollingMapLabel";
        //清空表
        sql_query.prepare(clear_sql);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
            db.close();
            return false;
        }
        else
        {
            return true;
        }
  }
}

bool dbSetPollingMapLabel(QString name,int iPicX,int iPicY,float fMapx,float fMapy,float omega,float theta,
                                int width,int height)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("robot");
    if(!db.open())
    {
        qDebug()<<db.lastError();
        qFatal("failed to connect.") ;
         db.close();
        return false;
    }
    else
    {
        //QSqlQuery类提供执行和操作的SQL语句的方法。
        //可以用来执行DML（数据操作语言）语句，如SELECT、INSERT、UPDATE、DELETE，
        //以及DDL（数据定义语言）语句，例如CREATE TABLE。
        //也可以用来执行那些不是标准的SQL的数据库特定的命令。
        QSqlQuery sql_query;
        QString create_sql = "CREATE TABLE IF NOT EXISTS PollingMapLabel (id INTEGER PRIMARY KEY,name varchar(50),xPix INTEGER"
                             ",yPix INTEGER,xMap FLOAT,yMap FLOAT,omega FLOAT,theta FLOAT,height INTEGER,width INTEGER)";
        QString select_max_sql = "select max(id) from PollingMapLabel";
        QString insert_sql = "insert into PollingMapLabel values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
        QString update_sql = "update PollingMapLabel set name = :name, xPix = :xPix, yPix = :yPix, xMap = :xMap, yMap = :yMap, omega = :omega,"
                             "theta = :theta ,height = :height,width = :width where id = :id";
        QString select_sql = "select id, name, xPix, yPix, xMap, yMap, omega, theta, height, width from PollingMapLabel";
        QString select_all_sql = "select * from PollingMapLabel";
        QString delete_sql = "delete from PollingMapLabel where id = ?";
        QString clear_sql = "delete from PollingMapLabel";
        sql_query.prepare(create_sql);
        if(!sql_query.exec())
        {
             qDebug()<<sql_query.lastError();
              db.close();
             return false;
        }
        else
        {
             qDebug()<<"table created!";
        }
        //查询最大id
        int max_id = 0;
        sql_query.prepare(select_max_sql);
        if(!sql_query.exec())
        {
             qDebug()<<sql_query.lastError();
        }
        else
        {
            while(sql_query.next())
            {
                max_id = sql_query.value(0).toInt();
                qDebug()<<QString("max id:%1").arg(max_id);
            }
        }

         //insert
        //插入数据
        sql_query.prepare(insert_sql);
        sql_query.addBindValue(max_id+1);
        sql_query.addBindValue(name);
        sql_query.addBindValue(iPicX);
        sql_query.addBindValue(iPicY);
        sql_query.addBindValue(fMapx);
        sql_query.addBindValue(fMapy);
        sql_query.addBindValue(omega);
        sql_query.addBindValue(theta);
        sql_query.addBindValue(height);
        sql_query.addBindValue(width);
        if(!sql_query.exec())
        {
             qDebug()<<sql_query.lastError();
        }
        else
        {
             qDebug()<<"inserted!";
        }
    }
}

Str_FixLabel dbReadPollingMapLabel(int id)
{
    Str_FixLabel strFixMapLabel;
    strFixMapLabel.name = "";
    strFixMapLabel.iPicX = 0;
    strFixMapLabel.iPicY = 0;
    strFixMapLabel.fMapX = 0;
    strFixMapLabel.fMapY = 0;
    strFixMapLabel.fMapTheta = 0;
    strFixMapLabel.fMapOmega = 0;
    strFixMapLabel.ilabelWidth = 0;
    strFixMapLabel.ilabelHeight = 0;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("robot");
    if(!db.open())
    {
        qDebug()<<db.lastError();
        qFatal("failed to connect.") ;
        db.close();
        return strFixMapLabel;
    }
    else
    {
        //QSqlQuery类提供执行和操作的SQL语句的方法。
        //可以用来执行DML（数据操作语言）语句，如SELECT、INSERT、UPDATE、DELETE，
        //以及DDL（数据定义语言）语句，例如CREATE TABLE。
        //也可以用来执行那些不是标准的SQL的数据库特定的命令。
        QSqlQuery sql_query;
        QString create_sql = "CREATE TABLE IF NOT EXISTS PollingMapLabel (id INTEGER PRIMARY KEY,name varchar(50),xPix INTEGER"
                             ",yPix INTEGER,xMap FLOAT,yMap FLOAT,omega FLOAT,theta FLOAT,height INTEGER,width INTEGER)";
        QString select_max_sql = "select max(id) from PollingMapLabel";
        QString insert_sql = "insert into PollingMapLabel values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
        QString update_sql = "update PollingMapLabel set name = :name, xPix = :xPix, yPix = :yPix, xMap = :xMap, yMap = :yMap, omega = :omega,"
                             "theta = :theta ,height = :height,width = :width where id = :id";
        QString select_sql = "select id, name, xPix, yPix, xMap, yMap, omega, theta, height, width from PollingMapLabel";
        QString select_all_sql = "select * from PollingMapLabel";
        QString select_sql_id = "select * from PollingMapLabel where id = ?";
        QString delete_sql = "delete from PollingMapLabel where id = ?";
        QString clear_sql = "delete from PollingMapLabel";
        //查询最大id
        int max_id = 0;
        sql_query.prepare(select_max_sql);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
            db.close();
            return strFixMapLabel;
        }
        else
        {
            while(sql_query.next())
            {
                max_id = sql_query.value(0).toInt();
                qDebug()<<QString("max id:%1").arg(max_id);
            }
        }
        //查询部分数据
        //        sql_query.prepare(update_sql);
        //        sql_query.bindValue(":name", "hyman");
        //        sql_query.bindValue(":id", 1);
        if(max_id > 0)
        {
//            select_sql.arg(1);
            sql_query.prepare(select_sql_id);
            sql_query.addBindValue(id);

            if(!sql_query.exec())
            {
                qDebug()<<sql_query.lastError();
            }
            else
            {
//                while(sql_query.next())
//                {
                sql_query.next(); //不是很理解为什么需要.next
                int id = sql_query.value("id").toInt();
                strFixMapLabel.name = sql_query.value("name").toString();
                strFixMapLabel.iPicX = sql_query.value("xPix").toInt();
                strFixMapLabel.iPicY = sql_query.value("yPix").toInt();
                strFixMapLabel.fMapX = sql_query.value("xMap").toFloat();
                strFixMapLabel.fMapY = sql_query.value("yMap").toFloat();
                strFixMapLabel.fMapOmega = sql_query.value("omega").toInt();
                strFixMapLabel.fMapTheta = sql_query.value("theta").toInt();
                strFixMapLabel.ilabelWidth = sql_query.value("width").toInt();
                strFixMapLabel.ilabelHeight = sql_query.value("height").toInt();

                qDebug()<<QString("id:%1 name:%2 xPix:%3").arg(id).arg( strFixMapLabel.name).arg(strFixMapLabel.iPicX);
                db.close();
                return strFixMapLabel;
            }
        }
        else
        {
            return strFixMapLabel;
        }
    }
}

bool readPollingLabelPosition()
{
    int imax;
    Str_FixLabel strFixLabel;
    tagPollingMapLabelList.clear();
    imax = dbReadPollingMapLabelMax();
    for(int i = 0; i < imax ; i++)
    {
        strFixLabel = dbReadPollingMapLabel(i + 1);
        tagPollingMapLabelList.append(strFixLabel);
    }
    return true;
}

int dbReadPollingMapLabelMax()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("robot");
    if(!db.open())
    {
        qDebug()<<db.lastError();
        qFatal("failed to connect.") ;
        db.close();
        return 0;
    }
    else
    {
        //QSqlQuery类提供执行和操作的SQL语句的方法。
        //可以用来执行DML（数据操作语言）语句，如SELECT、INSERT、UPDATE、DELETE，
        //以及DDL（数据定义语言）语句，例如CREATE TABLE。
        //也可以用来执行那些不是标准的SQL的数据库特定的命令。
        QSqlQuery sql_query;
        QString select_max_sql = "select max(id) from PollingMapLabel";

        //查询最大id
        int max_id = 0;
        sql_query.prepare(select_max_sql);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
            db.close();
            return 0;
        }
        else
        {
            while(sql_query.next())
            {
                max_id = sql_query.value(0).toInt();
                qDebug()<<QString("max id:%1").arg(max_id);
            }
        }
        return max_id;
    }
}

#endif //end of #ifndef _AGVPARA_C
//===================================================================
//No more
//===================================================================
