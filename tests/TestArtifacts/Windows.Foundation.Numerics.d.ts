// Copyright (c) Microsoft Corporation. 
// Licensed under the MIT License.

//tslint:disable

declare namespace Windows.Foundation.Numerics {
    interface Matrix3x2 {
        m11: number;
        m12: number;
        m21: number;
        m22: number;
        m31: number;
        m32: number;
    }

    interface Matrix4x4 {
        m11: number;
        m12: number;
        m13: number;
        m14: number;
        m21: number;
        m22: number;
        m23: number;
        m24: number;
        m31: number;
        m32: number;
        m33: number;
        m34: number;
        m41: number;
        m42: number;
        m43: number;
        m44: number;
    }

    interface Plane {
        normal: Windows.Foundation.Numerics.Vector3;
        d: number;
    }

    interface Quaternion {
        x: number;
        y: number;
        z: number;
        w: number;
    }

    interface Rational {
        numerator: number;
        denominator: number;
    }

    interface Vector2 {
        x: number;
        y: number;
    }

    interface Vector3 {
        x: number;
        y: number;
        z: number;
    }

    interface Vector4 {
        x: number;
        y: number;
        z: number;
        w: number;
    }

}