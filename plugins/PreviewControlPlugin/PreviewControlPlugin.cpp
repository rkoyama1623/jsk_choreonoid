/**
   @author Kunio Kojima
*/
#include "PreviewControlPlugin.h"

using namespace boost;
using namespace cnoid;
using namespace std;

bool PreviewControlPlugin::initialize()
{
    mBar = new PreviewControlBar(this);
    addToolBar(mBar);

    return true;
}

// 球面からZ座標を計算
double PreviewControlPlugin::calcZFromSphere(const Vector3d centerPos, const Vector3d pos, const double radius)
{
    return centerPos.z() + sqrt( pow(radius,2) - pow(pos.x()-centerPos.x(), 2) - pow(pos.y()-centerPos.y(), 2) );
}

// 腰位置を可動域内に修正
void PreviewControlPlugin::modifyWaistIntoRange
(Vector3d& waistPos, const Vector3d lFootPos, const Vector3d rFootPos, const Vector3d lHipPos, const Vector3d rHipPos, const double legLength)
{
    // void modifyWaistIntoRange( Position waistPos, const Vector3d lFootPos, const Vector3d rFootPos, const Vector3d lHipPos, const Vector3d rHipPos, const double legLength ){
    // const double ratio = 1;
    const double ratio = 0.99;
    double z = waistPos.z();
    waistPos.z() = min( waistPos.z(),
                        min( waistPos.z() - lHipPos.z() + calcZFromSphere( lFootPos, lHipPos, legLength*ratio ),
                             waistPos.z() - rHipPos.z() + calcZFromSphere( rFootPos, rHipPos, legLength*ratio ) ) );
    // cout << " lhippos:" << lHipPos.z() << "  sphere z(l):" << calcZFromSphere( lFootPos, lHipPos, legLength*ratio ) << endl;
    // cout << " rhippos:" << rHipPos.z() << "  sphere z(r):" << calcZFromSphere( rFootPos, rHipPos, legLength*ratio ) << endl;
    if(waistPos.z() < z) cout << "down waist " << z-waistPos.z() << endl;
}

// 予見制御
void PreviewControlPlugin::execControl()
{
    cout << "\x1b[31m" << "Start Preview Control" << "\x1b[m" << endl;

    BodyItemPtr bodyItemPtr;
    BodyPtr body;
    PoseSeqItemPtr poseSeqItemPtr;
    PoseSeqPtr poseSeqPtr;
    BodyMotionItemPtr bodyMotionItemPtr;
    BodyMotionPtr motion;
    if(!getSelectedPoseSeqSet(bodyItemPtr, body, poseSeqItemPtr, poseSeqPtr, bodyMotionItemPtr, motion)) return;

    // 足先リンク名取得
    Link *lFootLink, *rFootLink;
    UtilPlugin::getFootLink( &lFootLink, &rFootLink, body );

    boost::filesystem::path poseSeqPath(poseSeqItemPtr->filePath());
    cout << " parent_path:" << poseSeqPath.parent_path().string() << " basename:" << getBasename(poseSeqPath) << endl;

    JointPathPtr jpl = getCustomJointPath( body, body->rootLink(), lFootLink );
    JointPathPtr jpr = getCustomJointPath( body, body->rootLink(), rFootLink );

    // 脚長さ計算
    for(int i = 0; i < jpl->numJoints(); ++i) jpl->joint(i)->q() =0;
    body->calcForwardKinematics();
    double legLength = ( jpl->joint(0)->p() - jpl->endLink()->p() ).norm();
    cout << "legLength: " << legLength << endl;

    // motion生成
    cout << "numFrames: " << motion->numFrames() << endl;
    if(motion->numFrames() == 0){
        generateBodyMotionFromBar(body, poseSeqItemPtr, bodyMotionItemPtr);
    }else{
        cout << "Need not generate motion" << endl;
    }

    double dt = 1.0/motion->frameRate(), max_tm = motion->numFrames() / (double)motion->frameRate();

    // 目標zmpを計算
    PoseSeqInterpolatorPtr poseSeqInterpolatorPtr = poseSeqItemPtr->interpolator();
    Vector3SeqPtr refZmpSeqPtr;
    // refZmpSeqPtr.reset( new Vector3Seq() );
    // refZmpSeqPtr->setNumFrames(motion->numFrames(), true);
    // for(size_t i = 0; i < static_cast<size_t>(round(max_tm / dt)); i++){
    //   poseSeqInterpolatorPtr->seek( i * dt );

    //   // poseSeqのZMP
    //   refZmpSeqPtr->at(i).x() = (*(poseSeqInterpolatorPtr->ZMP())).x();
    //   refZmpSeqPtr->at(i).y() = (*(poseSeqInterpolatorPtr->ZMP())).y();
    //   refZmpSeqPtr->at(i).z() = (*(poseSeqInterpolatorPtr->ZMP())).z();
    // }

    // refZmpSeqPtr = motion->getOrCreateExtraSeq<Vector3Seq>("ZMP");// motionのZMP
    refZmpSeqPtr = motion->extraSeq<Vector3Seq>("ZMP");// motionのZMP Vector3SeqはZMPSeqでもいい??
    cout << "Finished generating ref zmp seq" << endl;

    Vector3SeqPtr initialZMPSeqPtr;
    initialZMPSeqPtr.reset( new Vector3Seq() );
    initialZMPSeqPtr->setNumFrames(motion->numFrames(), true);
    calcZMP( body, motion, initialZMPSeqPtr );// 入力動作のzmpの計算

    // 目標zmp・初期zmp・初期重心軌道書き出し
    {
        stringstream ss;
        ss << poseSeqPath.stem().string() << "_PC";
        if(mBar->dialog->saveParameterInFileNameCheck.isChecked()) ss << mBar->dialog->getParamString();
        ss << "_" << motion->frameRate() << ".dat";
        ofstream ofs(((filesystem::path) poseSeqPath.parent_path() / ss.str()).string().c_str());
        Vector3d lastPosVec = body->rootLink()->p();
        Vector3d lastVelVec = VectorXd::Zero(3);
        ofs << "time initZMPx initZMPy initZMPz refZMPx refZMPy refZMPz initCMx initCMy initCMz rootPosX rootPosY rootPosZ rootVelX rootVelY rootVelZ rootAccX rootAccY rootAccZ" << endl;
        for(int i = 0; i < motion->numFrames(); ++i){
            motion->frame(i) >> *body;
            body->calcForwardKinematics();
            body->calcCenterOfMass();
      
            Vector3d velVec = (body->rootLink()->p() - lastPosVec)/dt;
            Vector3d accVec = (velVec - lastVelVec)/dt;
            lastPosVec = body->rootLink()->p(); lastVelVec = velVec;

            ofs << i*dt
                << " " << initialZMPSeqPtr->at(i).transpose() << " " << refZmpSeqPtr->at(i).transpose() << " " << body->centerOfMass().transpose()
                << " " << body->rootLink()->p().transpose() << " " << velVec.transpose() << " " << accVec.transpose() << endl;
        }
        ofs.close();
    }

    // 予見制御収束ループ
    Vector3SeqPtr zmpSeqPtr;
    string modeStr = mBar->dialog->controlModeCombo->currentText().toStdString();
    cout << "control mode: " << modeStr << endl;
    string dfMode = mBar->dialog->controlModeCombo->itemText(DynamicsFilter).toStdString();
    string tpMode = mBar->dialog->controlModeCombo->itemText(TrajectoryPlanning).toStdString();
    for(int loopNum = 0; loopNum < 5; ++loopNum){
        cout << "loop: " << loopNum << endl;

        zmpSeqPtr.reset( new Vector3Seq() );
        zmpSeqPtr->setNumFrames(motion->numFrames(), true);
        calcZMP( body, motion, zmpSeqPtr );// 実際のzmpの計算

        // 予見制御用の実際のzmpと目標zmp、誤差zmp、時刻tmを計算
        std::queue<hrp::Vector3> ref_zmp_list;
        std::deque<double> tm_list;
        for(size_t i = 0; i < static_cast<size_t>(round(max_tm / dt)); ++i){
            // tm_list
            double tmp_tm = i * dt;
            tm_list.push_back(tmp_tm);
            // ref_zmp_list
            hrp::Vector3 ref_v;// 目標zmp
            ref_v << refZmpSeqPtr->at(i).x(), refZmpSeqPtr->at(i).y(), refZmpSeqPtr->at(i).z();
        
            hrp::Vector3 v;// 実際のzmp
            v << zmpSeqPtr->at(i).x(), zmpSeqPtr->at(i).y(), zmpSeqPtr->at(i).z();

            // 目標値リストに入力
            if(i+1 < static_cast<size_t>(round(max_tm / dt))){
                if(modeStr == dfMode){
                    ref_zmp_list.push(ref_v - v);
                }else if(modeStr == tpMode){
                    ref_zmp_list.push(ref_v);
                }else{
                    cout << "No such controle mode" << endl;
                    return;
                }
            }else{
                ref_zmp_list.push(ref_zmp_list.back());// 最後は1つ前の値と同じ
            }
        }
        cout << "Finished generating ref zmp list" << endl;
    
        Vector3SeqPtr diffCMSeqPtr;
        diffCMSeqPtr.reset( new Vector3Seq() );
        diffCMSeqPtr->setNumFrames(motion->numFrames(), true);

        Vector3SeqPtr cartZMPSeqPtr;
        cartZMPSeqPtr.reset(new Vector3Seq());
        cartZMPSeqPtr->setNumFrames(motion->numFrames(), true);

        // preview_dynamics_filter<preview_control> df(dt, 0.8, ref_zmp_list.front());
        rats2::preview_dynamics_filter<rats2::extended_preview_control> df(dt, 0.8, ref_zmp_list.front());
        double cart_zmp[3], refzmp[3];
        bool r = true;
        size_t index = 0;
        while (r) {
            hrp::Vector3 p, x;// p: current_refzmp   x: refcog
            // r = df.update(p, x, ref_zmp_list.front(), ref_frame_list.front(), !ref_zmp_list.empty());
            r = df.update(p, x, ref_zmp_list.front(), !ref_zmp_list.empty());
            if (r) {
                df.get_cart_zmp(cart_zmp);
                df.get_current_refzmp(refzmp);

                x.z() = 0;
                diffCMSeqPtr->at(index) = x;

                cartZMPSeqPtr->at(index) << cart_zmp[0], cart_zmp[1], cart_zmp[2];

                ++index;
            } else if ( !ref_zmp_list.empty() ) r = true;
            if (!ref_zmp_list.empty()){
                ref_zmp_list.pop();
            }
        }
        cout << "Finished calculating ref diff centroid" << endl;

        for(int i = 0; i < motion->numFrames(); ++i){
            Vector3d lFootPos,rFootPos, lHipPos, rHipPos, dCM;
            Matrix3 lFootR,rFootR;
            motion->frame(i) >> *body;
            body->calcForwardKinematics();// 状態更新
            dCM = diffCMSeqPtr->at(i) - body->calcCenterOfMass();
            dCM.z() = 0;

            lFootPos = lFootLink->p(); lFootR = lFootLink->R();// 足先位置取得
            rFootPos = rFootLink->p(); rFootR = rFootLink->R();

            if(modeStr == dfMode){
                body->rootLink()->p() += 0.7*diffCMSeqPtr->at(i);// 腰位置修正 ゲインを掛けるだけでは微妙
            }else if(modeStr == tpMode){
                body->rootLink()->p() += dCM;
            }
            body->calcForwardKinematics();// 状態更新
            lHipPos = jpl->joint(0)->p(); rHipPos = jpr->joint(0)->p();

            Vector3 tmpVec = body->rootLink()->p();
            modifyWaistIntoRange( tmpVec, lFootPos, rFootPos, lHipPos, rHipPos, legLength );
            body->rootLink()->p() = tmpVec;// 腰位置修正は要改良
            body->calcForwardKinematics();// 状態更新

            if(!jpl->calcInverseKinematics(lFootPos,lFootR)) cout << "\x1b[31m" << i*dt << " lfoot IK fail" << "\x1b[m" << endl;
            if(!jpr->calcInverseKinematics(rFootPos,rFootR)) cout << "\x1b[31m" << i*dt << " rfoot IK fail" << "\x1b[m" << endl;

            motion->frame(i) << *body;
        }
        cout << "Finished modifying waist position" << endl;

        // zmpファイル書き出し
        {
            stringstream ss;
            ss << poseSeqPath.stem().string() << "_PC";
            if(mBar->dialog->saveParameterInFileNameCheck.isChecked()) ss << mBar->dialog->getParamString();
            ss << "_" << motion->frameRate() << "fps_" << loopNum << ".dat";
            ofstream ofs;
            ofs.open(((filesystem::path) poseSeqPath.parent_path() / ss.str()).string().c_str());

            calcZMP( body, motion, zmpSeqPtr );
            ofs << "time  cartZMPx cartZMPy cartZMPz refCMx refCMy refCMz actZMPx actZMPy actZMPz actCMx actCMy actCMz rootPosX rootPosY rootPosZ" << endl;
            for(int i = 0; i < motion->numFrames(); ++i){
                motion->frame(i) >> *body;
                body->calcForwardKinematics();
                body->calcCenterOfMass();
                ofs << i*dt
                    << " " << cartZMPSeqPtr->at(i).transpose() << " " << diffCMSeqPtr->at(i).transpose()
                    << " " << zmpSeqPtr->at(i).transpose() << " " << body->centerOfMass().transpose() << " " << body->rootLink()->p().transpose() << endl;
            }
            ofs.close();
        }
        cout << "Finished recording ZMP trajectory" << endl;

    }// modifying loop

    // motionに出力動作ZMPを代入
    Vector3SeqPtr finalZMPSeqPtr = motion->extraSeq<Vector3Seq>("ZMP");// motionのZMP
    for(int i = 0; i < motion->numFrames(); ++i){
        finalZMPSeqPtr->at(i) = zmpSeqPtr->at(i);
    }

    cout << "Finished assigning ZMP to motion class" << endl;

    cout << "\x1b[31m" << "Finished Preview Control" << "\x1b[m" << endl << endl;
}

CNOID_IMPLEMENT_PLUGIN_ENTRY(PreviewControlPlugin)
