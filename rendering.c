#include "objects.h"
#include "helpers.h"
#include <math.h>

/*
 *float orthoIntersectD(int y, line l){ //returns 0 if there's no intersection
 *    if(l.y1 == l.y2){return 0;}
 *    if( (l.y1 < y)==(l.y2 < y) ){return 0;}
 *    if(l.x1 == l.x2){return l.x1;}
 *    return (y - l.y1 - ob_slope(l) * l.x1 ) / ob_slope(l);
 *}
 */

/*
 *void rn_orthoTest(unsigned char* screen) {
 *    for(int i=0;i<k_nPixels*3;i++){ screen[i]=0; }
 *    float closeD, newD;
 *    line *l = salloc(sizeof(line));
 *    for(int y=0;y<k_nPixels;y++) {
 *        closeD = k_drawD;
 *        for(int obj=0;;obj++) {
 *            if(ob_levelTest[obj*3] == terminator) {break;} //Check for termination
 *            for(int line=0;;line++) {
 *                if(ob_isTerminating(objFtype(ob_levelTest[obj*3])[line])){break;}
 *                *l = objFtype(ob_levelTest[obj*3])[line];
 *                ob_realifyLine(l, (ob_levelTest + obj*3 +1));
 *                if( (newD = orthoIntersectD(y, *l)) == 0){continue;}
 *                if(newD < closeD){
 *                    closeD=newD;
 *                    screen[y*3 +0]=l->r;
 *                    screen[y*3 +1]=l->g;
 *                    screen[y*3 +2]=l->b;
 *                }
 *            }
 *        }
 *    }
 *    free(l);
 *    return;
 *}
 */

/*
 *void rn_perspTest(int camX, int camY, double camT, unsigned char *screen) {
 *    int camD = 500; //This is arbitrary, it just has to be set large enough so there aren't roundoff errors
 *    for(int i=0;i<k_nPixels*3;i++){ screen[i]=0; }
 *    double a = camD/cos((pi/180)*k_FOV/2);
 *    double xa = a*cos((pi/180)*camT);
 *    double ya = a*sin((pi/180)*camT);
 *    double beta = 90+k_FOV/2+camT;
 *    line c; //Holds the ray eminating from the camera. Calculated based on what pixel we're on
 *        c.x1 = camX;
 *        c.y1 = camY;
 *    float closeD, newD;
 *    line l;
 *    for(int y=0;y<k_nPixels;y++) {
 *        double y_ = y*2*camD*tan(pi/180*k_FOV/2.0)/k_nPixels; //unit conversion from pixels to "true distance"
 *        c.x2 = (int)(xa + y_*cos((pi/180)*beta));
 *        c.y2 = (int)(ya + y_*sin((pi/180)*beta));
 *        closeD=k_drawD*k_drawD;
 *        for(int obj=0;;obj++) {
 *            if(ob_levelTest[obj*3] == terminator) {break;} //Check for termination
 *            for(int line=0;;line++) {
 *                if(ob_isTerminating(objFtype(ob_levelTest[obj*3])[line])){break;}
 *                l = objFtype(ob_levelTest[obj*3])[line];
 *                ob_realifyLine(&l, (ob_levelTest + obj*3 +1));
 *                if((newD = ob_intersectD(c, l) ) == 0) {continue;}
 *                if(newD < closeD) {
 *                    closeD=newD;
 *                    screen[y*3 +0]=l.r;
 *                    screen[y*3 +1]=l.g;
 *                    screen[y*3 +2]=l.b;
 *                }
 *            }
 *        }
 *    }
 *    return;
 *}
 */

/*
 *int rn_dimTest(line **renderArr){//returns the number of lines
 *    line l;
 *    int nLines=0;
 *    *renderArr = resalloc(*renderArr, sizeof(line) * k_nMaxLinesPerObj * k_nMaxObj);
 *    for(int obj=0;;obj++) {
 *        if(ob_levelTest[obj*3] == terminator) {break;} //Check for termination
 *        for(int line=0;;line++) {
 *            if(ob_isTerminating(objFtype(ob_levelTest[obj*3])[line])){break;}
 *            l = objFtype(ob_levelTest[obj*3])[line];
 *            ob_realifyLine(&l, (ob_levelTest + obj*3 +1));
 *            (*renderArr)[nLines++] = l;
 *        }
 *    }
 *    return nLines;
 *}
 */

int rn_dimFworld(line **renderArr, struct world w){//returns the number of lines
    line l;
    int nLines=0;
    *renderArr = resalloc(*renderArr, sizeof(line) * (k_nMaxLinesPerObj * k_nMaxObj + 1000));
    int camD = 30;
    double a = camD/cos((pi/180)*k_FOV/2);
    double xa = w.camX + a*cos((pi/180)*w.camT);
    double ya = w.camY + a*sin((pi/180)*w.camT);
    double xb = w.camX + a*cos((pi/180)*(w.camT+k_FOV));
    double yb = w.camY + a*sin((pi/180)*(w.camT+k_FOV));
    (*renderArr)[nLines++] = (line) {.x1 = w.camX, .y1= w.camY, .x2=xa, .y2=ya, .r=155, .g=55, .b=155};
    (*renderArr)[nLines++] = (line) {.x1 = w.camX, .y1= w.camY, .x2=xb, .y2=yb, .r=155, .g=55, .b=155};
    (*renderArr)[nLines++] = (line) {.x1 = xa, .y1= ya, .x2=xb, .y2=yb, .r=155, .g=55, .b=155};
    (*renderArr)[nLines++] = (line) {.x1 = -k_drawD, .x2 = k_drawD, .y1=0, .y2=0, .r=55, .g=55, .b=55};
    (*renderArr)[nLines++] = (line) {.y1 = -k_drawD, .y2 = k_drawD, .x1=0, .x2=0, .r=55, .g=55, .b=55};
    /*
     *for(int i=0; i<100; i++){
     *    int r = k_fogD;
     *    (*renderArr)[nLines++] = (line) {.x1 = w.camX + r*cos(2*pi/100*i), .x2 = w.camX + r*cos(2*pi/100*(i+1)), .y1 = w.camY + r*sin(2*pi/100*i), .y2 = w.camY + r*sin(2*pi/100*(i+1)), .r = 55, .g = 55, .b = 55};
     *}
     */

    for(int obj=0;;obj++) {
        if(w.scene[obj*3] == terminator) {break;} //Check for termination
        for(int line=0;;line++) {
            if(ob_isTerminating(objFtype(w.scene[obj*3])[line])){break;}
            l = objFtype(w.scene[obj*3])[line];
            ob_realifyLine(&l, (w.scene + obj*3 +1));
            (*renderArr)[nLines++] = l;
        }
    }
    (*renderArr)[nLines] = (line) {.y1=0, .y2=0, .x1=0, .x2=0, .r=0, .g=0, .b=0};
    return nLines;
}

void rn_perspFworld(unsigned char *screen, struct world w){
    int camD = 500; //This is arbitrary, it just has to be set large enough so there aren't roundoff errors
    for(int i=0;i<k_nPixels*3;i++){ screen[i]=0; }
    double a = camD/cos((pi/180)*k_FOV/2);
    double xa = w.camX + a*cos((pi/180)*w.camT);
    double ya = w.camY + a*sin((pi/180)*w.camT);
    double beta = 90+k_FOV/2+w.camT;
    line c; //Holds the ray eminating from the camera. Calculated based on what pixel we're on
        c.x1 = w.camX;
        c.y1 = w.camY;
    float closeD, newD;
    line l;
    for(int y=0;y<k_nPixels;y++) {
        double y_ = y*2*camD*tan(pi/180*k_FOV/2.0)/k_nPixels; //unit conversion from pixels to "true distance"
        c.x2 = (int)(xa + y_*cos((pi/180)*beta));
        c.y2 = (int)(ya + y_*sin((pi/180)*beta));
        closeD=k_drawD*k_drawD;
        for(int obj=0;;obj++) {
            if(w.scene[obj*3] == terminator) {break;} //Check for termination
            if(w.scene[obj*3] == objPlayer) {continue;} //don't render playerBox
            for(int line=0;;line++) {
                l = objFtype(w.scene[obj*3])[line];
                if(ob_isTerminating(l)){break;}
                ob_realifyLine(&l, (w.scene + obj*3 +1));
                if((newD = ob_intersectD(c, l) ) == 0) {continue;}
                if(newD < closeD) {
                    closeD=newD;
                    screen[y*3 +0]=l.r;
                    screen[y*3 +1]=l.g;
                    screen[y*3 +2]=l.b;
                }
            }
        }
    }
    return;
}

void rn_perspFworld_v(unsigned char *screen, struct world w, line **lineArr){
    bool rays = false; //true for fun visual effects
    int camD = 500; //This is arbitrary, it just has to be set large enough so there aren't roundoff errors
    for(int i=0;i<k_nPixels;i++){
        screen[i*3]=107;
        screen[i*3+1]=136;
        screen[i*3+2]=255;
    }
    *lineArr = resalloc(*lineArr, sizeof(line) * (k_nMaxLinesPerObj * k_nMaxObj + k_nPixels + 1000));
        int nLines=0;
        do{nLines++;}while(!ob_isTerminating((*lineArr)[nLines]));
    double a = camD/cos((pi/180)*k_FOV/2);
    double xa = w.camX + a*cos((pi/180)*w.camT);
    double ya = w.camY + a*sin((pi/180)*w.camT);
    double beta = 90+k_FOV/2+w.camT;
    line c; //Holds the ray eminating from the camera. Calculated based on what pixel we're on
        c.x1 = w.camX;
        c.y1 = w.camY;
    float closeD, newD;
    line l;
    for(int y=0;y<k_nPixels;y++) {
        double y_ = y*2*camD*tan(pi/180*k_FOV/2.0)/k_nPixels; //unit conversion from pixels to "true distance"
        c.x2 = (int)(xa + y_*cos((pi/180)*beta));
        c.y2 = (int)(ya + y_*sin((pi/180)*beta));
        closeD=k_drawD*k_drawD;
        //Verbose
        if(rays){
            c.r = 200;
            c.g = 200;
            c.b = 200;
            (*lineArr)[nLines] = c;
        }
        //Verbose
        for(int obj=0;;obj++) {
            if(w.scene[obj*3] == terminator) {break;} //Check for termination
            if(w.scene[obj*3] == objPlayer) {continue;} //don't render playerBox
            for(int li=0;;li++) {
                l = objFtype(w.scene[obj*3])[li];
                if(ob_isTerminating(l)){break;}
                ob_realifyLine(&l, (w.scene + obj*3 +1));
                if((newD = ob_intersectD(c, l) ) == 0) {continue;}
                if(newD < closeD) {
                    closeD=newD;
                    float intX, intY;
                    ob_intersect(c, l, &intX, &intY);
                    screen[y*3 +0]=l.r;
                    screen[y*3 +1]=l.g;
                    screen[y*3 +2]=l.b;
                    //Verbose
                    int xx, yy;
                    ob_intersect(c, l, &intX, &intY);
                    if(rays){
                        c.x2 = intX;
                        c.y2 = intY;
                        c.r = 250;
                        c.g = 250;
                        c.b = 250;
                        (*lineArr)[nLines] = c;
                    }
                    else{
                        if(l.x1==l.x2){xx = intX; yy = intY+1;}
                        else{xx = intX+1; yy=intY+1*ob_slope(l);}
                        (*lineArr)[nLines] = (line){.x1 = (int) intX, .y1 = (int) intY, .x2 = xx, .y2 = yy, .r = fmin(l.r+80, 255), .g = fmin(l.g+80, 255), .b = fmin(l.b+80, 255)};
                    }
                    (*lineArr)[nLines+1] = (line) {.y1=0, .y2=0, .x1=0, .x2=0, .r=0, .g=0, .b=0};
                    //Verbose
                }
            }
        }
        //Verbose
        if(!ob_isTerminating((*lineArr)[nLines]) || rays) {nLines++;}
        //Verbose
    }
    return;
}

