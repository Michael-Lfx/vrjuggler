#ifndef QUATERNION_CLASS_SELF_TEST_H
#define QUATERNION_CLASS_SELF_TEST_H

#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <Math/vjMath.h>
#include <Math/vjQuat.h>

/*****************************************************************
 tests out the functionality expected of vj::Quat
*******************************************************************/

namespace vrjTest
{

class QuatSelfTest : public TestCase
{
public:
   QuatSelfTest( std::string name ) : TestCase (name)
   {
   }
   
   
   virtual ~QuatSelfTest()
   {
   }

   void identTest()
   {
      // the default identity is currently the multiplicative identity [1,0,0,0]
      // not the addition identity... [0,0,0,0]
      // consider allowing both kinds of identity to be made in the quat API...
      vjQuat quat;
      quat.makeIdent();
      assertTest( quat == vjQuat::identity() );
      assertTest( quat[VJ_W] == 1.0f && quat[VJ_X] == 0.0f && 
                  quat[VJ_Y] == 0.0f && quat[VJ_Z] == 0.0f );
   }
      
   void makeRotTest()
   {
      //std::cout<<"make sure that normalize doesn't change the rotation...\n"<<std::flush;
      //std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<std::flush;

      vjQuat q1, q2, q3, q4;
      q1.makeRot( vjMath::deg2rad( 45.0f ), 0,1,0 );
      q2.makeRot( vjMath::deg2rad( 90.0f ), 1,0,0 );
      q3 = q1; 
      q4 = q2;
      q3.normalize();
      q4.normalize();

      /*
      std::cout<<"before: \n"<<std::flush;
      std::cout<<"q1:           "<<q1<<" .. "<<q1[VJ_W]<<" "<<q1[VJ_X]<<" "<<q1[VJ_Y]<<" "<<q1[VJ_Z]<<"\n"<<std::flush;
      std::cout<<"q2:           "<<q2<<" .. "<<q2[VJ_W]<<" "<<q2[VJ_X]<<" "<<q2[VJ_Y]<<" "<<q2[VJ_Z]<<"\n"<<std::flush;
      std::cout<<"after: \n"<<std::flush;
      std::cout<<"q1:           "<<q1<<" .. "<<q1[VJ_W]<<" "<<q1[VJ_X]<<" "<<q1[VJ_Y]<<" "<<q1[VJ_Z]<<"\n"<<std::flush;
      std::cout<<"q2:           "<<q2<<" .. "<<q2[VJ_W]<<" "<<q2[VJ_X]<<" "<<q2[VJ_Y]<<" "<<q2[VJ_Z]<<"\n"<<std::flush;
      std::cout<<"\n"<<std::flush;
      */
   
            // make sure that normalize doesn't change the rotation...         
      assertTest( q1.isEqual( q3, VJ_QUAT_EPSILON ) );
      assertTest( q2.isEqual( q4, VJ_QUAT_EPSILON ) );
   }

   void xformVecSweepTest()
   {
      //std::cout<<"xform vector by quat (sweep over range of rotations)\n"<<std::flush;
      //std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<std::flush;
      vjQuat q;
      vjVec3 v(0,0,1);
      q.makeIdent();

      //std::cout<<"Should go from 0,0,1 to 0,-1,0 to 0,0,-1 ....\n"<<std::flush;
      for (float x = 0; x <= 180.0f; x+=90)
      {
         q.makeRot( vjMath::deg2rad( x ), 1,0,0 );

         vjVec3 result = q * v;
         if (x == 0)
            assertTest( result[0] < 0.001 && result[1] < 0.001 && result[2] > 0.999 );
         if (x == 90)
            assertTest( result[0] < 0.001 && result[1] < -0.999 && result[2] < 0.001 );
         if (x == 180)
            assertTest( result[0] < 0.001 && result[1] < 0.001 && result[2] < -0.999 );
         //std::cout<<result<<"\n"<<std::flush;
      }
      //std::cout<<"\n"<<std::flush;
   }

   void makeRotGetRotSanityTest()
   {
      //std::cout<<"testing sanity of makeRot and getRot, then makeRot again...\n"<<std::flush;
      //std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<std::flush;

      for (float i = -360; i <= 360; i+=20)
      {
         vjQuat q, q2;
         float rad, x, y, z;
         q.makeRot( vjMath::deg2rad(i),1,0,0 );
         //std::cout<<i<<") "<<q<<" .. "<<q[VJ_W]<<" "<<q[VJ_X]<<" "<<q[VJ_Y]<<" "<<q[VJ_Z]<<" compare to: "<<std::flush;

         q.getRot( rad, x, y, z );
         q2.makeRot( rad, x, y, z );
         //std::cout<<q2[VJ_W]<<" "<<q2[VJ_X]<<" "<<q2[VJ_Y]<<" "<<q2[VJ_Z]<<"\n"<<std::flush;

         // make i a positive by x*360, store in b.  needed so I can use the % operator with neg values..
         assertTest( q.isEqual( q2, VJ_QUAT_EPSILON ) );

         float b = i;
         while ( b < 0.0f)
         {
            b += 360.0f;
         }      

         if (i >= 0.0f)
         {
            float c = vjMath::abs( (float)((int)(vjMath::round(b))%360) );
            float temp1 = vjMath::rad2deg(rad); temp1 = (float)((int)(vjMath::round(temp1))%360);
            float temp2 = c + 0.5;
            float temp3 = vjMath::rad2deg(rad); temp3 = (float)((int)(vjMath::round(temp3))%360);
            float temp4 = c - 0.5;
            //   std::cout<<temp1<<" "<<temp2<<" "<<temp3<<" "<<temp4<<" "<<vjMath::rad2deg(rad)<<" "<<b<<"\n"<<std::flush;

            assertTest( temp1 <= temp2 && temp3 >= temp4 );     
         }    
      }
      //std::cout<<"\n"<<std::flush;
   }

   void scalarMultTest()
   {
      //std::cout<<"testing scaling...\n"<<std::flush;
      //std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<std::flush;

      for (float i = -360; i <= 360; i+=20)
      {
         vjQuat q, q2;
         float rad, x, y, z;
         q.makeRot( vjMath::deg2rad(i),1,0,0 );
         q2.scaleAngle( q, 0.5 );
         q2.getRot( rad, x, y, z );

         // make i a positive by x*360, store in b.  needed so I can use the % operator with neg values..
          //std::cout<<vjMath::rad2deg(rad)<<" "<<(0.5 * i)<<"\n"<<std::flush;
          float half_quat_angle = vjMath::rad2deg(rad);
          float half_original_angle = (0.5 * i);
          half_quat_angle = vjMath::abs( half_quat_angle );
          half_original_angle = vjMath::abs( half_original_angle );

          float tol = 0.1;
          assertTest( (half_original_angle <= (half_quat_angle + tol)) &&
               (half_original_angle >= (half_quat_angle - tol)) );
      }

      //std::cout<<"\n"<<std::flush;
   }

   void simpleQuatProductTest()
   {
      //std::cout<<"Quat product: no rotation * rotation\n"<<std::flush;
      //std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<std::flush;

      vjQuat q1, q2, q3;
      q1.makeRot( 0, 1,0,0 );
      q2.makeRot( vjMath::deg2rad( 90.0f ), 1,0,0 );
      //std::cout<<"[0 rotation] * [90deg about x] should be [90deg about x]\n"<<std::flush;
      //std::cout<<"q1:           "<<q1<<" .. "<<q1[VJ_W]<<" "<<q1[VJ_X]<<" "<<q1[VJ_Y]<<" "<<q1[VJ_Z]<<"\n"<<std::flush;
      //std::cout<<"q2:           "<<q2<<" .. "<<q2[VJ_W]<<" "<<q2[VJ_X]<<" "<<q2[VJ_Y]<<" "<<q2[VJ_Z]<<"\n"<<std::flush;

      // first rotate by q2, then by q1
      q3 = q1 * q2;

      //std::cout<<"q3 = q1 * q2: "<<q3<<" .. "<<q3[VJ_W]<<" "<<q3[VJ_X]<<" "<<q3[VJ_Y]<<" "<<q3[VJ_Z]<<"\n"<<std::flush;
      //std::cout<<"\n"<<std::flush;

      vjVec3 v( 0,1,0 ), r;
      r = q3 * v;
      //std::cout<<r<<"\n"<<std::flush;
      //std::cout<<"\n"<<std::flush;

      assertTest( r[2] > 0.99f );
   }

   // another quaternion product test
   void secondQuatProductTest()
   {
      //std::cout<<"Quat product: rotation * rotation\n"<<std::flush;
      //std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<std::flush;
      vjQuat q1, q2, q3;
      q1.makeRot( vjMath::deg2rad( 45.0f ), 0,1,0 );
      q2.makeRot( vjMath::deg2rad( 90.0f ), 1,0,0 );
      //std::cout<<"[45 about Y] * [90 about X] should be [90deg about .7,0,-.7]\n"<<std::flush;
      //std::cout<<"q1:           "<<q1<<" .. "<<q1[VJ_W]<<" "<<q1[VJ_X]<<" "<<q1[VJ_Y]<<" "<<q1[VJ_Z]<<"\n"<<std::flush;
      //std::cout<<"q2:           "<<q2<<" .. "<<q2[VJ_W]<<" "<<q2[VJ_X]<<" "<<q2[VJ_Y]<<" "<<q2[VJ_Z]<<"\n"<<std::flush;

      // first rotate by q2, then by q1
      q3 = q1 * q2;
      //q3.normalize();

      //std::cout<<"q3 = q1 * q2: "<<q3<<" .. "<<q3[VJ_W]<<" "<<q3[VJ_X]<<" "<<q3[VJ_Y]<<" "<<q3[VJ_Z]<<"\n"<<std::flush;
      //std::cout<<"\n"<<std::flush;

      vjVec3 v( 0,1,0 ), r;
      r = q3 * v;
      //std::cout<<r<<"\n"<<std::flush;
      //std::cout<<"\n"<<std::flush;

      assertTest( r[0] > 0.7 && r[2] > 0.7 );
   }

   void xformVecTest()
   {
      //std::cout<<"xform vec by quat\n"<<std::flush;
      //std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<std::flush;
      vjQuat q1, q2;//, q3;
      q1.makeRot( vjMath::deg2rad( 45.0f ), 0,-1,0 );
      q2.makeRot( vjMath::deg2rad( 45.0f ), 1,0,0 );

      vjVec3 v( 0,1,0 ), r;

      // (should not move)
      r = q1 * v;
      //std::cout<<r<<"\n"<<std::flush;
      assertTest( r[1] > 0.999 );

      // rotate forward
      r = q2 * v;
      //std::cout<<r<<"\n"<<std::flush;
      assertTest( r[1] > 0.7 && r[2] > 0.7 );

      //std::cout<<"\n"<<std::flush;
   }


   // just trying stuff out... TODO think of a good add test...
   void specialCases()
   {
      std::cout<<"special cases\n"<<std::flush;
      std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<std::flush;

      vjQuat q( 1, 0, -0.000313354, 0 );
      float rad, x, y, z;
      q.getRot( rad, x, y, z );

      std::cout<<rad<<" "<<x<<" "<<y<<" "<<z<<"\n"<<std::flush;

      // testing...
      double half_angle = 0.000626708 * 0.5f;
      double sin_half_angle = vjMath::sin( half_angle );
      double w = vjMath::cos( half_angle );
      std::cout<<half_angle<<" "<<sin_half_angle<<" "<<w<<"\n"<<std::flush;


      //vjQuat qq( 0,0,0,0 );

      std::cout<<"make sure that makeRot(180,0,1,0) doesn't yield [0,0,0,0]\n"<<std::flush;
      vjQuat qqq;
      qqq.makeRot( vjMath::deg2rad( 180.0f ), 0, 1, 0 );
      std::cout<<"qqq: "<<qqq<<" .. "<<qqq[VJ_W]<<" "<<qqq[VJ_X]<<" "<<qqq[VJ_Y]<<" "<<qqq[VJ_Z]<<"\n"<<std::flush;
      assertTest( qqq[VJ_W] < VJ_QUAT_EPSILON && qqq[VJ_W] > -VJ_QUAT_EPSILON &&
              qqq[VJ_X] == 0.0f &&
              qqq[VJ_Y] > (1.0f - VJ_QUAT_EPSILON) && qqq[VJ_Y] < (1.0f + VJ_QUAT_EPSILON) &&
              qqq[VJ_Z] == 0.0f );

      std::cout<<"\n"<<std::flush;
   }

   // just trying stuff out... TODO think of a good way to test this ability...
   // TODO: could we use a vector (pure quat) to scale a quat in a 
   //        more robust way?  pretty sure we can... rewrite scaleAngle to use this
   //        method instead....
   void vectorScaleTest()
   {
      std::cout<<"vector scale of a rotation quaternion\n"<<std::flush;
      std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<std::flush;

      std::cout<<"vector scale of a rotation quaternion yields an invalid rotation (only unit-quats define a rotation).\n"<<std::flush;

      // non normalized
      for (int x = 0; x < 360; ++x)
      {
         vjQuat q, q2;
         q.makeRot( vjMath::deg2rad( (float)x ), 0.7, 0, 0.7 );
         q2.mult( q, 0.5 );

         // no longer a valid rotation (non-unit length).  
         // should be normalized to define an actual rotation.
         assertTest( q2.length() <= (1.0f - VJ_QUAT_EPSILON) ||
                 q2.length() >= (1.0f + VJ_QUAT_EPSILON) );

         float rad, i,j,k;
         q2.getRot( rad, i,j,k );
         std::cout<<x<<") "<<vjMath::rad2deg( rad )<<" "<<i<<" "<<j<<" "<<k<<"\n"<<std::flush;
      }

      std::cout<<"If normalized, then the scaled quat is equal to the original."<<std::flush;
      // normalized.  
      for (int x = 0; x < 360; ++x)
      {
         vjQuat q, q2;
         q.makeRot( vjMath::deg2rad( (float)x ), 0.7, 0, 0.7 );
         q2.mult( q, 0.5 );
         q2.normalize();

         assertTest( q2.isEqual( q, VJ_QUAT_EPSILON ) );
      }        

      std::cout<<"\n"<<std::flush;
   }

   // just trying stuff out... TODO think of a good add test...
   void quatAdd()
   {
      std::cout<<"quat add\n"<<std::flush;
      std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<std::flush;


      {
         vjQuat q, q1, q2;
         q1.makeRot( vjMath::deg2rad( 90.0f ), 1, 0, 0 );
         q2.makeRot( vjMath::deg2rad( 180.0f ), 1, 0, 0 );

         q.add( q1, q2 );
         q.normalize();

         std::cout<<q<<"\n"<<std::flush;
      }

      vjQuat q, q1;//, q2;
      q1.makeRot( vjMath::deg2rad( 90.0f ), 1, 0, 0 );

      q = q1 * q;

      std::cout<<q<<"\n"<<std::flush;
   }

   // using a vector to increment rotation, much like angular velocity
   // here angular velocity is defined as a vjVec3 "w", using right hand rule
   // things spin at |w| speed and on the w vector.
   //
   // just trying stuff out... TODO think of a good way to test this ability...
   void pureQuatMultTest()
   {
      std::cout<<"pureQuatMultTest (angular velocity test)\n"<<std::flush;
      std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"<<std::flush;

      for (float x = 0; x < 100; ++x)
      {
         vjQuat q1, q2, wq;
         vjVec3 w(0, x, 0); // angular velocity
         wq.makePure( w );
         q1.makeRot( vjMath::deg2rad( 90.0f ), 0, 1, 0 );

         q2 = wq * q1;
         //////q2.normalize();  // don't normalize, will not work!!!!!!!!!

         std::cout<<"["<<q2<<"] == ["<<wq<<"] * ["<<q1<<"]\n"<<std::flush;
      }

      vjQuat q1, wq;
      vjVec3 ww(0, 56, 0);
      wq.makePure( ww );
      q1.makeRot( vjMath::deg2rad( 90.0f ), 0, 1, 0 );

      const float& w1( wq[VJ_W] );
      const float& w2( q1[VJ_W] );
      vjVec3 v1( wq[VJ_X], wq[VJ_Y], wq[VJ_Z] );
      vjVec3 v2( q1[VJ_X], q1[VJ_Y], q1[VJ_Z] );

      float w = w1 * w2 - v1.dot( v2 );
      vjVec3 v = (w1 * v2) + (w2 * v1) + v1.cross( v2 );

      std::cout<<"["<<w<<", "<<v[0]<<", "<<v[1]<<", "<<v[2]<<"] == ["<<wq<<"] * ["<<q1<<"]\n"<<std::flush;
      std::cout<<(w1 * v2)<<"  == (w1 * v2) \n"<<std::flush;
      std::cout<<(w2 * v1)<<"  == (w2 * v1) \n"<<std::flush;
      std::cout<<v1.cross( v2 )<<"  == v1.cross( v2 ) \n"<<std::flush;
      std::cout<<v1<<" dot "<<v2<<" == "<<v1.dot( v2 )<<"\n"<<std::flush;
   }

   //assertTest( a != b.code() );
   
   
   static Test* suite()
   {
      TestSuite *test_suite = new TestSuite ("QuatSelfTest");

      test_suite->addTest( new TestCaller<QuatSelfTest>("identity test", &QuatSelfTest::identTest));        
      test_suite->addTest( new TestCaller<QuatSelfTest>("xformVecTest", &QuatSelfTest::xformVecTest));        
      test_suite->addTest( new TestCaller<QuatSelfTest>("xformVecSweepTest", &QuatSelfTest::xformVecSweepTest));        
      test_suite->addTest( new TestCaller<QuatSelfTest>("makeRotGetRotSanityTest", &QuatSelfTest::makeRotGetRotSanityTest));        
      test_suite->addTest( new TestCaller<QuatSelfTest>("scalarMultTest", &QuatSelfTest::scalarMultTest));        
      test_suite->addTest( new TestCaller<QuatSelfTest>("simpleQuatProductTest", &QuatSelfTest::simpleQuatProductTest));        
      test_suite->addTest( new TestCaller<QuatSelfTest>("secondQuatProductTest", &QuatSelfTest::secondQuatProductTest));        
      test_suite->addTest( new TestCaller<QuatSelfTest>("makeRotTest", &QuatSelfTest::makeRotTest));        
      //test_suite->addTest( new TestCaller<QuatSelfTest>("specialCases", &QuatSelfTest::specialCases));
      //test_suite->addTest( new TestCaller<QuatSelfTest>("vectorScaleTest", &QuatSelfTest::vectorScaleTest));
      //test_suite->addTest( new TestCaller<QuatSelfTest>("quatAdd", &QuatSelfTest::quatAdd));
      //test_suite->addTest( new TestCaller<QuatSelfTest>("pureQuatMultTest", &QuatSelfTest::pureQuatMultTest));
      return test_suite;
   }
};

}

#endif