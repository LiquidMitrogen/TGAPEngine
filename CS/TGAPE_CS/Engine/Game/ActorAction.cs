using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace TGAPE_CS.Engine.Game
{
    //TODO:maybe change some data structures and collection to optimize searching for a keyframe
    public class ActionBoneKeyframe
    {
        public uint boneId;
        public List<Keyframe> keyframes;//must be ordered by frame
        public ActionBoneKeyframe(uint boneId, List<Keyframe> keyframes)
        {
            this.boneId = boneId;
            this.keyframes = keyframes;
        }
    };
    public class ActorAction
    {
        protected List<ActionBoneKeyframe> boneKeyframes;

        public uint rangeStart, rangeEnd;
        public string name;

        public List<ActionBoneKeyframe> BoneKeyframes
        {
            get => boneKeyframes;
            set => boneKeyframes = value;
        }

        public ActorAction(string name, uint rangeStart, uint rangeEnd, List<ActionBoneKeyframe> boneKeyframes) 
        {
            this.name = name;
            this.rangeStart = rangeStart;
            this.rangeEnd = rangeEnd;
            this.boneKeyframes = boneKeyframes;
        }
        //TODO:rename this method to something more intuitive
        public Keyframe GetQuatLoc(uint boneId, uint frame)
        {
            var desiredBoneKeyframes = boneKeyframes.FirstOrDefault(x => x.boneId == boneId);
            if(desiredBoneKeyframes == null || desiredBoneKeyframes.keyframes.Count == 0)
            {
                Debugger.Break();
                return new Keyframe(frame, Vector3.Zero, Quaternion.Identity);//there are no keyframes
            }
            if (desiredBoneKeyframes.keyframes.Count == 1)
            {
                return desiredBoneKeyframes.keyframes.First();//frame doesent matter since there is only one keyframe
            }
            if(frame < desiredBoneKeyframes.keyframes.First().frame)
            {
                Debugger.Break();
                return desiredBoneKeyframes.keyframes.First();//desired frame is before first frame
            }
            Keyframe? tmpPreviousKeyframe = null;
            foreach(var currentKeyframe in desiredBoneKeyframes.keyframes)
            {
                if (currentKeyframe.frame >= frame)
                {
                    if (currentKeyframe.frame == frame)
                    {
                        return currentKeyframe;//desired frame is a keyframe
                    }
                    else
                    {
                        var interpolationRatio = (float)(frame - tmpPreviousKeyframe.Value.frame) 
                            / (float)(currentKeyframe.frame - tmpPreviousKeyframe.Value.frame);
                        var locationInterpolated = Vector3.Lerp(tmpPreviousKeyframe.Value.location, 
                            currentKeyframe.location, 
                            interpolationRatio);
                        var rotationInterpolated = Quaternion.Slerp(tmpPreviousKeyframe.Value.rotation,
                            currentKeyframe.rotation,
                            interpolationRatio);
                        return new Keyframe(frame, locationInterpolated, rotationInterpolated);
                    }
                }
                tmpPreviousKeyframe = currentKeyframe;
            }
            Debugger.Break();
            return desiredBoneKeyframes.keyframes.Last();//desired frame is after last keyyframe

        }

    }

}

