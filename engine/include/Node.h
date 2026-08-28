#pragma once

class Node {
public:

    Node(
        int id,
        double x,
        double y
    );

    int getId() const;

    double getX() const;
    double getY() const;

    void setFixedX(bool fixed);
    void setFixedY(bool fixed);

    bool isFixedX() const;
    bool isFixedY() const;

    void setForceX(double force);
    void setForceY(double force);

    double getForceX() const;
    double getForceY() const;

private:

    int id;

    double x;
    double y;

    bool fixedX;
    bool fixedY;

    double forceX;
    double forceY;
};