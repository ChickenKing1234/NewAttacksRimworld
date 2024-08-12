using RimWorld;
using Verse;
using System.Collections.Generic;

namespace MyUnarmedAttacksMod
{
    public class ModInitializer : Mod
    {
        public ModInitializer(ModContentPack content) : base(content)
        {
            // Initialization code
        }
    }

    [DefOf]
    public static class DefOf
    {
        public static DamageDef UnarmedStrangle;
        public static DamageDef UnarmedPunch;
        public static DamageDef UnarmedKick;
        public static DamageDef UnarmedHeadbutt;
        public static DamageDef UnarmedThrow;

        public static HediffDef AsphyxiationHediff;
        public static HediffDef PunchHediff;
        public static HediffDef KickHediff;
        public static HediffDef HeadbuttHediff;
        public static HediffDef ThrowHediff;
    }

    public class JobDriver_UnarmedStrangle : JobDriver
    {
        private const int MaxStranglesLowSkill = 4;
        private const int MaxStranglesHighSkill = 3;
        private int currentStrangles;
        private bool isHighSkill;
        private bool canStrangle;

        public override bool TryMakePreToilReservations(bool forced = false)
        {
            return true;
        }

        protected override IEnumerable<Toil> MakeNewToils()
        {
            this.FailOn(() => job.targetA.Thing == null);

            Pawn target = (Pawn)job.targetA.Thing;
            if (target != null)
            {
                isHighSkill = pawn.skills.GetSkill(SkillDefOf.Melee).Level >= 10;
                if (pawn.gender == Gender.Female && target.gender == Gender.Male)
                {
                    canStrangle = pawn.skills.GetSkill(SkillDefOf.Melee).Level >= target.skills.GetSkill(SkillDefOf.Melee).Level + 5;
                }
                else
                {
                    canStrangle = true;
                }
            }

            Toil strangleToil = new Toil
            {
                tickAction = () =>
                {
                    Pawn targetPawn = (Pawn)job.targetA.Thing;
                    if (targetPawn != null)
                    {
                        if (!canStrangle)
                        {
                            EndJobWith(JobCondition.Incompletable);
                            return;
                        }

                        if (currentStrangles >= (isHighSkill ? MaxStranglesHighSkill : MaxStranglesLowSkill))
                        {
                            targetPawn.health.AddHediff(HediffDefOf.Death);
                            EndJobWith(JobCondition.Incompletable);
                        }
                        else
                        {
                            var hediff = targetPawn.health.hediffSet.GetFirstHediffOfDef(DefOf.AsphyxiationHediff);
                            if (hediff == null)
                            {
                                targetPawn.health.AddHediff(DefOf.AsphyxiationHediff);
                            }
                            else
                            {
                                hediff.Severity += 0.05f;
                                if (hediff.Severity >= 1.0f)
                                {
                                    targetPawn.health.AddHediff(HediffDefOf.Death);
                                    EndJobWith(JobCondition.Incompletable);
                                }
                            }

                            targetPawn.stances.stunner.StunFor(30 * 60, pawn);

                            currentStrangles++;
                        }
                    }
                },
                defaultCompleteMode = ToilCompleteMode.Never,
                defaultDuration = 200
            };

            Toil repeatToil = new Toil
            {
                initAction = () =>
                {
                    currentStrangles = 0;
                },
                defaultCompleteMode = ToilCompleteMode.Never,
                defaultDuration = 600
            };

            yield return repeatToil;
            yield return strangleToil;
        }
    }

    public class JobDriver_UnarmedPunch : JobDriver
    {
        public override bool TryMakePreToilReservations(bool forced = false)
        {
            return true;
        }

        protected override IEnumerable<Toil> MakeNewToils()
        {
            Toil punchToil = new Toil
            {
                tickAction = () =>
                {
                    Pawn target = (Pawn)job.targetA.Thing;
                    if (target != null)
                    {
                        target.TakeDamage(new DamageInfo(DefOf.UnarmedPunch, 5));
                    }
                },
                defaultCompleteMode = ToilCompleteMode.Never,
                defaultDuration = 100
            };
            yield return punchToil;
        }
    }

    public class JobDriver_UnarmedKick : JobDriver
    {
        public override bool TryMakePreToilReservations(bool forced = false)
        {
            return true;
        }

        protected override IEnumerable<Toil> MakeNewToils()
        {
            Toil kickToil = new Toil
            {
                tickAction = () =>
                {
                    Pawn target = (Pawn)job.targetA.Thing;
                    if (target != null)
                    {
                        target.TakeDamage(new DamageInfo(DefOf.UnarmedKick, 10));
                    }
                },
                defaultCompleteMode = ToilCompleteMode.Never,
                defaultDuration = 150
            };
            yield return kickToil;
        }
    }

    public class JobDriver_UnarmedHeadbutt : JobDriver
    {
        public override bool TryMakePreToilReservations(bool forced = false)
        {
            return true;
        }

        protected override IEnumerable<Toil> MakeNewToils()
        {
            Toil headbuttToil = new Toil
            {
                tickAction = () =>
                {
                    Pawn target = (Pawn)job.targetA.Thing;
                    if (target != null)
                    {
                        target.TakeDamage(new DamageInfo(DefOf.UnarmedHeadbutt, 15));
                    }
                },
                defaultCompleteMode = ToilCompleteMode.Never,
                defaultDuration = 120
            };
            yield return headbuttToil;
        }
    }

    public class JobDriver_UnarmedThrow : JobDriver
    {
        public override bool TryMakePreToilReservations(bool forced = false)
        {
            return true;
        }

        protected override IEnumerable<Toil> MakeNewToils()
        {
            Toil throwToil = new Toil
            {
                tickAction = () =>
                {
                    Pawn target = (Pawn)job.targetA.Thing;
                    if (target != null)
                    {
                        // Determine the damage of the throw
                        int damage = 20;
                        // Apply damage and a stun effect
                        target.TakeDamage(new DamageInfo(DefOf.UnarmedThrow, damage));
                        target.stances.stunner.StunFor(15 * 60, pawn);
                    }
                },
                defaultCompleteMode = ToilCompleteMode.Never,
                defaultDuration = 150
            };
            yield return throwToil;
        }
    }
}
