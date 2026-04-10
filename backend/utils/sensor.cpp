#pragma once

#include "../ascas.h"

class SensorUtils {
    private:
    
    const std::string sessionStrings = 
        "0123456789abcdef";
    std::string generateSession(int len = 64) {
        srand(clock3());
        std::string session = "";
        for (int i = 0; i < len; i++) session += sessionStrings[rand() % sessionStrings.size()];
        return session;
    }

    void pushMessage(int uid, int sid, double value, time_t t) {
        Connection conn = Client("/tmp/ascas/sensorList.sock").connect();
        conn.send("send");
        conn.send(std::to_string(uid));
        conn.send(std::to_string(sid) + " " + std::to_string(value) + " " + std::to_string(t));
        int cnt = stoi(conn.recv());
        conn.close();

        conn = Client("/tmp/ascas/sensorDetails.sock").connect();
        conn.send("send");
        conn.send(std::to_string(sid));
        conn.send(std::to_string(value) + " " + std::to_string(t));
        cnt = stoi(conn.recv());
        conn.close();
    }

    public:

    std::pair<int, std::string> create(Sensor sensor) {
        quick_mysqli_connect();

        std::string sensorId = generateSession();

        mysqli_execute(
            mysql,
            "INSERT INTO sensors (uid, title, unit, sensorId) VALUES (%d, \"%s\", \"%s\", \"%s\")",
            sensor.owner.uid,
            quote_encode(sensor.title).c_str(),
            quote_encode(sensor.unit).c_str(),
            sensorId.c_str()
        );

        return {
            stoi(mysqli_query(
                mysql,
                "SELECT MAX(id) AS maxid FROM sensors WHERE sensorId = \"%s\"",
                sensorId.c_str()
            )[0]["maxid"]),
            sensorId  
        };
    }

    std::vector<Sensor> list(int uid) {
        quick_mysqli_connect();

        auto res = mysqli_query(
            mysql,
            "SELECT A.*, A.id AS sid, B.* FROM sensors AS A "
            "INNER JOIN users AS B ON B.id = A.uid "
            "WHERE uid = %d",
            uid
        );

        std::vector<Sensor> ans;
        for (int i = 0; i < res.size(); i++) {
            auto data = mysqli_query(
                mysql,
                "SELECT value, createdAt FROM sensor_data WHERE sid = %d ORDER BY createdAt DESC LIMIT 10",
                stoi(res[i]["sid"])
            );
            std::vector<double> values;
            std::vector<time_t> createdAt;
            for (int j = data.size() - 1; j >= 0; j--) 
                values.push_back(stod(data[j]["value"])),
                createdAt.push_back(stol(data[j]["createdAt"]));
            ans.push_back(Sensor({
                .sid = stoi(res[i]["sid"]),
                .owner = User({
                    .uid = stoi(res[i]["uid"]),
                    .name = res[i]["name"],
                    .email = res[i]["email"],
                    .isAdmin = stoi(res[i]["isAdmin"]) ? true : false
                }),
                .title = res[i]["title"],
                .unit = res[i]["unit"],
                .data = values,
                .createdAt = createdAt
            }));
        }

        return ans;
    }

    int exists(int sid) {
        quick_mysqli_connect();

        return stoi(mysqli_query(
            mysql,
            "SELECT COUNT(*) AS count FROM sensors WHERE id = %d",
            sid
        )[0]["count"]);
    }

    Sensor getSensorInfo(int sid) {
        quick_mysqli_connect();

        auto res = mysqli_query(
            mysql,
            "SELECT A.*, A.id AS sid, B.* FROM sensors AS A "
            "INNER JOIN users AS B ON B.id = A.uid "
            "WHERE A.id = %d",
            sid
        )[0];

        return Sensor({
            .sid = stoi(res["sid"]),
            .owner = User({
                .uid = stoi(res["uid"]),
                .name = res["name"],
                .email = res["email"],
                .isAdmin = stoi(res["isAdmin"]) ? true : false
            }),
            .title = res["title"],
            .unit = res["unit"],
            .data = {},
            .createdAt = {}
        });
    }

    int checkSensorId(std::string sensorId) {
        quick_mysqli_connect();

        auto res = mysqli_query(
            mysql,
            "SELECT id FROM sensors WHERE sensorId = \"%s\"",
            sensorId.c_str()
        );

        if (res.size() == 0) return 0;
        else return stoi(res[0]["id"]);
    }

    void pushValue(int sid, double value) {
        quick_mysqli_connect();

        time_t t = time(NULL);
        int uid = getSensorInfo(sid).owner.uid;

        mysqli_execute(
            mysql,
            "INSERT INTO sensor_data (sid, value, createdAt) VALUES (%d, %lf, %lld)",
            sid,
            value,
            t
        );

        pushMessage(uid, sid, value, t);
    }
    
    void remove(int sid) {
        quick_mysqli_connect();

        mysqli_execute(
            mysql,
            "DELETE FROM sensors WHERE id = %d",
            sid
        );

        mysqli_execute(
            mysql,
            "DELETE FROM sensor_data WHERE sid = %d",
            sid
        );
    }

    std::vector<std::pair<double, time_t> > query(int sid, time_t t, int count) {
        quick_mysqli_connect();

        auto res = mysqli_query(
            mysql,
            "SELECT value, createdAt FROM sensor_data WHERE sid = %d AND createdAt <= %lld ORDER BY createdAt DESC LIMIT %d",
            sid,
            t,
            count
        );

        std::vector<std::pair<double, time_t> > ans;
        for (int i = res.size() - 1; i >= 0; i--)
            ans.push_back({
                stod(res[i]["value"]),
                stol(res[i]["createdAt"])
            });
        
        return ans;
    }

    std::string regenerate(int sid) {
        quick_mysqli_connect();

        std::string sensorId = generateSession();
        mysqli_execute(
            mysql,
            "UPDATE sensors SET sensorId = \"%s\" WHERE id = %d",
            sensorId.c_str(),
            sid
        );

        return sensorId;
    }

    void edit(Sensor sensor) {
        quick_mysqli_connect();

        mysqli_execute(
            mysql,
            "UPDATE sensors SET title = \"%s\", unit = \"%s\" WHERE id = %d",
            quote_encode(sensor.title).c_str(),
            quote_encode(sensor.unit).c_str(),
            sensor.sid
        );
    }
}SensorUtils;