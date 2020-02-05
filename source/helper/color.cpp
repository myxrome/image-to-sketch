//
// Created by Aleksey on 28.04.2015.
//

#include <opencv2/imgproc.hpp>
#include "color.hpp"

namespace color {

    static int L_INDEX = 0;
    static int A_INDEX = 1;
    static int B_INDEX = 2;

    double calculateColorDistanceCIE76(Vec3f lab_color1, Vec3f lab_color2) {
        double delta_L = lab_color2[L_INDEX] - lab_color1[L_INDEX];
        double delta_a = lab_color2[A_INDEX] - lab_color1[A_INDEX];
        double delta_b = lab_color2[B_INDEX] - lab_color1[B_INDEX];

        return sqrt(pow(delta_L, 2) + pow(delta_a, 2) + pow(delta_b, 2));
    }

    double calculateC(double a, double b) {
        return sqrt((a * a) + (b * b));
    }

    double calculateG(double avg_C) {
        double avg_7_C = pow(avg_C, 7);
        return 0.5 * (1 - sqrt(avg_7_C / (avg_7_C + 6103515625.0)));
    }

    double calculateAp(double a, double g) {
        return (1 + g) * a;
    }

    double radianToDegree(double radian) {
        return radian * 57.2957795785523;
    }

    double degreeToRadian(double degree) {
        return degree / 57.2957795785523;
    }

    double calculateHp(double ap, double b) {
        double hp = 0;
        if ((ap != 0) || (b != 0)) {
            hp = radianToDegree(atan2(b, ap));
            if (b < 0) {
                hp += 360;
            }
        }
        return hp;
    }

    double calculateDelta_h(double Cp1, double Cp2, double hp1, double hp2) {
        double dh = 0;
        if ((Cp1 * Cp2) != 0) {
            dh = hp2 - hp1;
            if (dh > 180) {
                dh -= 360;
            }
            else if (dh < -180) {
                dh += 360;
            }
        }
        return dh;
    }

    double calculateDeltaH(double Cp1, double Cp2, double delta_h) {
        return 2 * sqrt(Cp1 * Cp2) * sin(degreeToRadian(delta_h / 2));
    }

    double calculateAvgH(double Cp1, double Cp2, double hp1, double hp2) {
        double avg_h = hp1 + hp2;
        if ((Cp1 * Cp2) != 0) {
            double dh = abs(hp1 - hp2);
            if (dh <= 180) {
                avg_h /= 2;
            }
            else {
                if (avg_h < 360) {
                    avg_h = (avg_h + 360) / 2;
                }
                else {
                    avg_h = (avg_h - 360) / 2;
                }
            }
        }
        return avg_h;
    }

    double calculateT(double avg_h) {
        return 1 - 0.17 * cos(degreeToRadian(avg_h - 30))
               + 0.24 * cos(degreeToRadian(2 * avg_h))
               + 0.32 * cos(degreeToRadian(3 * avg_h + 6))
               - 0.2 * cos(degreeToRadian(4 * avg_h - 63));
    }

    double calculateDeltaTheta(double avg_h) {
        return 30 * exp(-1 * pow((avg_h - 275) / 25, 2));
    }

    double calculateRc(double avg_Cp) {
        double avg_7_Cp = pow(avg_Cp, 7);
        return 2 * sqrt(avg_7_Cp / (avg_7_Cp + 6103515625.0));
    }

    double calculateSl(double avg_L) {
        double double_avg_L = pow(avg_L - 50, 2);
        return 1 + ((0.015 * double_avg_L) / sqrt(20 + double_avg_L));
    }

    double calculateSc(double avg_Cp) {
        return 1 + 0.045 * avg_Cp;
    }

    double calculateSh(double avg_Cp, double T) {
        return 1 + 0.015 * avg_Cp * T;
    }

    double calculateRt(double delta_theta, double Rc) {
        return -1 * sin(degreeToRadian(2 * delta_theta)) * Rc;
    }

/*
 * http://www.ece.rochester.edu/~gsharma/ciede2000/ciede2000noteCRNA.pdf
 */
    double calculateColorDistanceCIEDE2000(Vec3f lab_color1, Vec3f lab_color2) {
        /*
         * (2)
         */
        double C1 = calculateC(lab_color1[A_INDEX], lab_color1[B_INDEX]);
        double C2 = calculateC(lab_color2[A_INDEX], lab_color2[B_INDEX]);

        /*
         * (3)
         */
        double avg_C = (C1 + C2) / 2;

        /*
         * (4)
         */
        double g = calculateG(avg_C);
        /*
         * (5)
         */
        double ap1 = calculateAp(lab_color1[A_INDEX], g);
        double ap2 = calculateAp(lab_color2[A_INDEX], g);

        /*
         * (6)
         */
        double Cp1 = calculateC(ap1, lab_color1[B_INDEX]);
        double Cp2 = calculateC(ap2, lab_color2[B_INDEX]);

        /*
         * (7)
         */
        double hp1 = calculateHp(ap1, lab_color1[B_INDEX]);
        double hp2 = calculateHp(ap2, lab_color2[B_INDEX]);

        /*
         * (8)
         */
        double delta_L = lab_color2[L_INDEX] - lab_color1[L_INDEX];

        /*
         * (9)
         */
        double delta_Cp = Cp2 - Cp1;

        /*
         * (10)
         */
        double delta_h = calculateDelta_h(Cp1, Cp2, hp1, hp2);

        /*
         * (11)
         */
        double delta_H = calculateDeltaH(Cp1, Cp2, delta_h);

        /*
         * (12)
         */
        double avg_L = (lab_color1[L_INDEX] + lab_color2[L_INDEX]) / 2;

        /*
         * (13)
         */
        double avg_Cp = (Cp1 + Cp2) / 2;

        /*
         * (14)
         */
        double avg_h = calculateAvgH(Cp1, Cp2, hp1, hp2);

        /*
         * (15)
         */
        double T = calculateT(avg_h);

        /*
         * (16)
         */
        double delta_theta = calculateDeltaTheta(avg_h);

        /*
         * (17)
         */
        double Rc = calculateRc(avg_Cp);

        /*
         * (18)
         */
        double Sl = calculateSl(avg_L);

        /*
         * (19)
         */
        double Sc = calculateSc(avg_Cp);

        /*
         * (20)
         */
        double Sh = calculateSh(avg_Cp, T);

        /*
         * (21)
         */
        double Rt = calculateRt(delta_theta, Rc);

        /*
         * (22)
         */
        double delta_EL = delta_L / Sl;
        double delta_EC = delta_Cp / Sc;
        double delta_EH = delta_H / Sh;

        return sqrt(
                pow(delta_EL, 2) + pow(delta_EC, 2) + pow(delta_EH, 2)
                + Rt * delta_EC * delta_EH);
    }

    int colorToIndex(int b, int g, int r) {
        return (b * 65536) + (g * 256) + r;
    }

    int colorToIndex(Vec3b color) {
        return (color[0] * 65536) + (color[1] * 256) + color[2];
    }

    Vec3f fixLabColor(Vec3b color) {
        return Vec3f(color[0] * 100 / 255, color[1] - 128, color[2] - 128);
    }

    Mat createLabColorMat() {
        Mat bgr_result(1, 16777216, CV_8UC3);

        for (int b = 0; b < 256; ++b) {
            for (int g = 0; g < 256; ++g) {
                for (int r = 0; r < 256; ++r) {
                    bgr_result.at<Vec3b>(0, colorToIndex(b, g, r)) = Vec3b((uchar) b, (uchar) g, (uchar) r);
                }
            }
        }
        Mat lab_result;
        cvtColor(bgr_result, lab_result, COLOR_BGR2Lab);
        return lab_result;
    }

}