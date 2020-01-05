using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace TGAPE_CS.Engine.Math
{
    public static class MatrixHelper
    {
        public static Matrix4x4 ApplyLocalTranslation(Matrix4x4 matrix, Vector3 translation)
        {
            var result = matrix;
            result.M41 = matrix.M41
                    + matrix.M11 * translation.X
                    + matrix.M21 * translation.X
                    + matrix.M31 * translation.X;
            result.M42 = matrix.M42
                    + matrix.M12 * translation.Y
                    + matrix.M22 * translation.Y
                    + matrix.M32 * translation.Y;
            result.M43 = matrix.M43
                    + matrix.M13 * translation.Z
                    + matrix.M23 * translation.Z
                    + matrix.M33 * translation.Z;
            return result;
        }
    }
}
