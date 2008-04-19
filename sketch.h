
#ifndef __SKETCH_H
#define __SKETCH_H

class hGroup;
class hRequest;
class hEntity;
class hParam;

class Entity;
class Param;

class hEquation;
class Equation;

// All of the hWhatever handles are a 32-bit ID, that is used to represent
// some data structure in the sketch.
class hGroup {
public:
    // bits 15: 0   -- group index
    DWORD v;
};
class hRequest {
public:
    // bits 15: 0   -- request index
    DWORD   v;

    inline hEntity entity(int i);
    inline hParam param(int i);
};
class hEntity {
public:
    // bits 15: 0   -- entity index
    //      31:16   -- request index
    DWORD   v;

    inline hRequest request(void);
};
class hParam {
public:
    // bits 15: 0   -- param index
    //      31:16   -- request index
    DWORD       v;

    inline hRequest request(void);
};

// A set of requests. Every request must have an associated group.
class Group {
public:
    static const hGroup     HGROUP_REFERENCES;

    int         tag;
    hGroup      h;

    NameStr     name;

    char *DescriptionString(void);
};

// A user request for some primitive or derived operation; for example a
// line, or a 
class Request {
public:
    // Some predefined requests, that are present in every sketch.
    static const hRequest   HREQUEST_REFERENCE_XY;
    static const hRequest   HREQUEST_REFERENCE_YZ;
    static const hRequest   HREQUEST_REFERENCE_ZX;

    int         tag;
    hRequest    h;

    // Types of requests
    static const int CSYS_2D                = 100;
    static const int DATUM_POINT            = 101;
    static const int LINE_SEGMENT           = 200;

    int         type;

    hEntity     csys; // or Entity::NO_CSYS

    hGroup      group;

    NameStr     name;

    hParam AddParam(IdList<Param,hParam> *param, hParam hp);
    void Generate(IdList<Entity,hEntity> *entity, IdList<Param,hParam> *param);

    char *DescriptionString(void);
};

class Entity {
public:
    int         tag;
    hEntity     h;

    static const hEntity    NO_CSYS;

    static const int CSYS_2D                = 1000;
    static const int POINT_IN_3D            = 2000;
    static const int POINT_IN_2D            = 2001;
    static const int LINE_SEGMENT           = 3000;
    int         type;

    bool        symbolic;
    // The params are usually handles to the symbolic variables, but may
    // also be constants
    union {
        hParam      h[16];
        double      v[16];
    }           param;
    // Associated entities, e.g. the endpoints for a line segment
    hEntity     assoc[16];

    hEntity     csys;   // or Entity::NO_CSYS

    // Applies only for a CSYS_2D type
    void Get2dCsysBasisVectors(Vector *u, Vector *v);

    bool IsPoint(void);
    // Applies for any of the point types
    void GetPointExprs(Expr **x, Expr **y, Expr **z);
    Vector GetPointCoords(void);
    void ForcePointTo(Vector v);
    bool IsFromReferences(void);

    // Routines to draw and hit-test the representation of the entity
    // on-screen.
    struct {
        bool    drawing;
        Point2d mp;
        double  dmin;
    } dogd;
    void LineDrawOrGetDistance(Vector a, Vector b);
    void DrawOrGetDistance(void);
    void Draw(void);
    double GetDistance(Point2d mp);

    char *DescriptionString(void);
};

class Param {
public:
    int         tag;
    hParam      h;

    double      val;
    bool        known;

    void ForceTo(double v);
};


inline hEntity hRequest::entity(int i)
    { hEntity r; r.v = (v << 16) | i; return r; }
inline hParam hRequest::param(int i)
    { hParam r; r.v = (v << 16) | i; return r; }

inline hRequest hEntity::request(void)
    { hRequest r; r.v = (v >> 16); return r; }

inline hRequest hParam::request(void)
    { hRequest r; r.v = (v >> 16); return r; }


class hConstraint {
public:
    DWORD   v;
};

class Constraint {
public:
    static const int USER_EQUATION      = 10;
    static const int POINTS_COINCIDENT  = 20;
    static const int PT_PT_DISTANCE     = 30;
    static const int PT_LINE_DISTANCE   = 31;

    static const int HORIZONTAL         = 40;
    static const int VERTICAL           = 41;

    int         tag;
    hConstraint h;

    int         type;
    hGroup      group;

    // These are the parameters for the constraint.
    Expr        *exprA;
    Expr        *exprB;
    hEntity     ptA;
    hEntity     ptB;
    hEntity     ptC;
    hEntity     entityA;
    hEntity     entityB;

    // These define how the constraint is drawn on-screen.
    struct {
        Vector      offset;
    } disp;

    static hConstraint AddConstraint(Constraint *c);
    static void MenuConstrain(int id);
    
    struct {
        bool    drawing;
        Point2d mp;
        double  dmin;
    } dogd; // state for drawing or getting distance (for hit testing)
    double GetDistance(Point2d mp);
    void Draw(void);
    void DrawOrGetDistance(void);

    bool HasLabel(void);

    void Generate(IdList<Equation,hEquation> *l);
};

class hEquation {
public:
    DWORD v;
};

class Equation {
public:
    int         tag;
    hEquation   h;

    Expr        *e;
};


#endif
