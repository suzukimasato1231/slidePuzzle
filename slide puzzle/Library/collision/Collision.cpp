#include "Collision.h"
#include "../Helper/Vec.h"

using namespace DirectX;

void Collision::ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest)
{
	// pointがp0の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	float d1 = XMVector3Dot(p0_p1, p0_pt).m128_f32[0];
	float d2 = XMVector3Dot(p0_p2, p0_pt).m128_f32[0];

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// p0が最近傍
		*closest = triangle.p0;
		return;
	}

	// pointがp1の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p1_pt = point - triangle.p1;

	float d3 = XMVector3Dot(p0_p1, p1_pt).m128_f32[0];
	float d4 = XMVector3Dot(p0_p2, p1_pt).m128_f32[0];

	if (d3 >= 0.0f && d4 <= d3)
	{
		// p1が最近傍
		*closest = triangle.p1;
		return;
	}

	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// pointがp2の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p2_pt = point - triangle.p2;

	float d5 = XMVector3Dot(p0_p1, p2_pt).m128_f32[0];
	float d6 = XMVector3Dot(p0_p2, p2_pt).m128_f32[0];
	if (d6 >= 0.0f && d5 <= d6)
	{
		*closest = triangle.p2;
		return;
	}

	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	// pointは面領域の中にある。closestを重心座標を用いて計算する
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, DirectX::XMVECTOR* inter)
{
	// 中心点の距離の２乗 <= 半径の和の２乗　なら交差
	float dist = XMVector3LengthSq(sphereA.center - sphereB.center).m128_f32[0];

	float radius2 = sphereA.radius + sphereB.radius;
	radius2 *= radius2;

	if (dist <= radius2) {
		if (inter) {
			// Aの半径が0の時座標はBの中心　Bの半径が0の時座標はAの中心　となるよう補完
			float t = sphereB.radius / (sphereA.radius + sphereB.radius);
			*inter = XMVectorLerp(sphereA.center, sphereB.center, t);
		}
		return true;
	}

	return false;
}

bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter)
{
	// 座標系の原点から球の中心座標への距離から
	// 平面の原点距離を減算することで、平面と球の中心との距離が出る
	float dist = XMVector3Dot(sphere.center, plane.normal).m128_f32[0] - plane.distance;
	// 距離の絶対値が半径より大きければ当たっていない
	if (fabsf(dist) > sphere.radius)	return false;

	// 擬似交点を計算
	if (inter)
	{
		// 平面上の再接近点を、疑似交点とする
		*inter = -dist * plane.normal + sphere.center;                                                                                              +sphere.center;
	}

	return true;
}

bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter, DirectX::XMVECTOR* reject)
{
	XMVECTOR p;
	// 球の中心に対する最近接点である三角形上にある点pを見つける
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);
	// 点pと球の中心の差分ベクトル
	XMVECTOR v = p - sphere.center;
	// 距離の二乗を求める
	//（同じベクトル同士の内積は三平方の定理のルート内部の式と一致する）
	float distanceSquare = XMVector3Dot(v, v).m128_f32[0];
	// 球と三角形の距離が半径以下なら当たっていない
	if (distanceSquare > sphere.radius * sphere.radius)	return false;
	// 擬似交点を計算
	if (inter) {
		// 三角形上の最近接点pを疑似交点とする
		*inter = p;
	}
	// 押し出すベクトルを計算
	if (reject) {
		float ds = XMVector3Dot(sphere.center, triangle.normal).m128_f32[0];
		float dt = XMVector3Dot(triangle.p0, triangle.normal).m128_f32[0];
		float rejectLen = dt - ds + sphere.radius;
		*reject = triangle.normal * rejectLen;
	}
	return true;
}

bool Collision::CheckRay2Plane(const Ray& lay, const Plane& plane, float* distance, DirectX::XMVECTOR* inter)
{
	const float epsilon = 1.0e-5f;	// 誤差吸収用の微小な値

	float d1 = XMVector3Dot(plane.normal, lay.dir).m128_f32[0];
	// 裏面には当たらない
	if (d1 > -epsilon) {
		return false;
	}

	float d2 = XMVector3Dot(plane.normal, lay.start).m128_f32[0];
	float t = (plane.distance - d2) / d1;

	if (t < 0) return false;

	// 距離を書き込む
	if (distance) {
		*distance = t;
	}

	// 交点を計算
	if (inter) {
		*inter = lay.start + t * lay.dir;
	}

	return true;
}

bool Collision::CheckRay2Triangle(const Ray& lay, const Triangle& triangle, float* distance, DirectX::XMVECTOR* inter)
{
	// 三角形が乗っている平面を算出
	Plane plane;
	XMVECTOR interPlane;
	plane.normal = triangle.normal;
	plane.distance = XMVector3Dot(triangle.normal, triangle.p0).m128_f32[0];
	// レイと平面が当たっていなければ、当たっていない	
	if (!CheckRay2Plane(lay, plane, distance, &interPlane)) {
		return false;
	}
	// レイと平面が当たっていたので、距離と交点が書き込まれた

	// レイと平面の交点が三角形の内側にあるか判定
	const float epsilon = 1.0e-5f;	// 誤差吸収用の微小な値
	XMVECTOR m;
	// 辺p0_p1について
	XMVECTOR pt_p0 = triangle.p0 - interPlane;
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	m = XMVector3Cross(pt_p0, p0_p1);
	// 辺の外側
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) {
		return false;
	}

	// 辺p1_p2について
	XMVECTOR pt_p1 = triangle.p1 - interPlane;
	XMVECTOR p1_p2 = triangle.p2 - triangle.p1;
	m = XMVector3Cross(pt_p1, p1_p2);
	// 辺の外側
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) {
		return false;
	}

	// 辺p2_p0について
	XMVECTOR pt_p2 = triangle.p2 - interPlane;
	XMVECTOR p2_p0 = triangle.p0 - triangle.p2;
	m = XMVector3Cross(pt_p2, p2_p0);
	// 辺の外側
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) {
		return false;
	}

	if (inter) {
		*inter = interPlane;
	}

	// 内側なので、当たっている
	return true;
}

bool Collision::CheckRay2Sphere(const Ray& lay, const Sphere& sphere, float* distance, DirectX::XMVECTOR* inter)
{
	XMVECTOR m = lay.start - sphere.center;
	float b = XMVector3Dot(m, lay.dir).m128_f32[0];
	float c = XMVector3Dot(m, m).m128_f32[0] - sphere.radius * sphere.radius;
	// layの始点がsphereの外側にあり(c > 0)、layがsphereから離れていく方向を
	// 差している場合(b > 0)、当たらない
	if (c > 0.0f && b > 0.0f) {
		return false;
	}

	float discr = b * b - c;
	// 負の判別式はレイが球を外れていることに一致
	if (discr < 0.0f) {
		return false;
	}

	// レイは球と交差している。
	// 交差する最小の値tを計算
	float t = -b - sqrtf(discr);
	// tが負である場合、レイは球の内側から開始しているのでtをゼロにクランプ
	if (t < 0) t = 0.0f;

	if (distance) {
		*distance = t;
	}

	if (inter) {
		*inter = lay.start + t * lay.dir;
	}

	return true;
}

//球と球の当たり判定
bool Collision::CheckSphere2Sphere(const Sphere& sphere1, const Sphere& sphere2)
{
	//２つの球の中心座標を結ぶベクトルを求める
	XMVECTOR vec = sphere1.center - sphere2.center;

	//ベクトルの長さの２乗（２点間の距離の２乗）を求める
	float sqLength = vec.m128_f32[0] * vec.m128_f32[0] + vec.m128_f32[1] * vec.m128_f32[1] + vec.m128_f32[2] * vec.m128_f32[2];

	//2つの球の半径の合計を求める
	float r = sphere1.radius + sphere2.radius;

	//２点間の距離の２乗＜半径の合計の２乗なら２つの球は交差している
	return  sqLength < r* r;
}

bool Collision::CheckBox2Box(const Box& box1, const Box& box2)
{
	//x軸の判定
	if (box2.minPosition.m128_f32[0] <= box1.maxPosition.m128_f32[0] && box1.minPosition.m128_f32[0] <= box2.maxPosition.m128_f32[0])
	{//y軸
		if (box2.minPosition.m128_f32[1] <= box1.maxPosition.m128_f32[1] && box1.minPosition.m128_f32[1] <= box2.maxPosition.m128_f32[1])
		{
			//z軸の判定
			if (box2.minPosition.m128_f32[2] <= box1.maxPosition.m128_f32[2] && box1.minPosition.m128_f32[2] <= box2.maxPosition.m128_f32[2])
			{

				return 1;
			}
		}
	}
	return 0;
}


//球と直方体（AABB）の衝突判定
bool Collision::CheckSphere2Box(const Sphere& sphere, const Box& box)
{
	//まず、球の中心座標とAABBとの距離の２乗を求める
	float sqDistance = 0.0f;
	float pos;

	//x軸方向の距離の２乗を加算
	pos = sphere.center.m128_f32[0];
	if (pos < box.minPosition.m128_f32[0])
	{
		sqDistance += (box.minPosition.m128_f32[0] - pos) * (box.minPosition.m128_f32[0] - pos);
	}
	else if (pos > box.maxPosition.m128_f32[0])
	{
		sqDistance += (box.maxPosition.m128_f32[0] - pos) * (box.maxPosition.m128_f32[0] - pos);
	}

	//y軸方向の距離の２乗を加算
	pos = sphere.center.m128_f32[1];
	if (pos < box.minPosition.m128_f32[1])
	{
		sqDistance += (box.minPosition.m128_f32[1] - pos) * (box.minPosition.m128_f32[1] - pos);
	}
	else if (pos > box.maxPosition.m128_f32[1])
	{
		sqDistance += (box.maxPosition.m128_f32[1] - pos) * (box.maxPosition.m128_f32[1] - pos);
	}
	pos = sphere.center.m128_f32[2];
	//z軸方向の距離の２乗を加算
	if (pos < box.minPosition.m128_f32[2])
	{
		sqDistance += (box.minPosition.m128_f32[2] - pos) * (box.minPosition.m128_f32[2] - pos);
	}
	else if (pos > box.maxPosition.m128_f32[2])
	{
		sqDistance += (box.maxPosition.m128_f32[2] - pos) * (box.maxPosition.m128_f32[2] - pos);
	}


	//上記で求めた値＜半径の２乗なら、衝突している
	return sqDistance < sphere.radius* sphere.radius;
}

bool Collision::CheckSphere2Capsule(const Sphere& sphere, const Capsule& capsule)
{
	Vec3 SS = { sphere.center.m128_f32[0],sphere.center.m128_f32[1] ,sphere.center.m128_f32[2] };
	Vec3 CS = { capsule.startPosition.m128_f32[0], capsule.startPosition.m128_f32[1], capsule.startPosition.m128_f32[2] };
	Vec3 CE = { capsule.endPosition.m128_f32[0], capsule.endPosition.m128_f32[1], capsule.endPosition.m128_f32[2] };

	//１．カプセル内の線分のスタート位置からエンド位置へのベクトルを作る
	Vec3 vStartToEnd = CS - CE;

	//2.1.のベクトルを単位ベクトル（normalize）し、nに用意する
	Vec3 n = vStartToEnd;
	n.normalize();

	//3.Ps->Pcへのベクトルと２．で求めたnとの内積を計算する
	//（するとnを何倍すればベクトルPs->Pnになるか倍率（t）が求まる）

	float t = n.dot(Vec3(CS - SS));

	//4.Ps->Pnベクトルを求めておく、また、Pnの座標を求めておく
	Vec3 vPsPn = n * t;
	Vec3 posPn = CS - vPsPn;

	//5.比率t/(Ps->Peの長さ)を求める
	float lengthRate = t / vStartToEnd.length();

	//6.lengthRate<0,0<=lengthRate<=1,1<lengthRateで場合分け
	float distance;
	if (lengthRate < 0.0f)
	{
		distance = (CS - SS).length() - capsule.radius;
	}
	else if (lengthRate <= 1.0f)
	{
		distance = (posPn - SS).length() - capsule.radius;
	}
	else
	{
		distance = (CE - SS).length() - capsule.radius;
	}

	return distance < sphere.radius;
}

bool Collision::CheckCapsule2Capsule(const Capsule &capsule1, const Capsule& capsule2)
{
	//線分と線分の距離を調べる　結果は距離の２乗の値である
	float sqDistance = sqDistanceSegmentSegment(capsule1.startPosition, capsule1.endPosition, capsule2.startPosition, capsule2.endPosition);


	//２つのカプセルの半径の和を求める
	float r = capsule1.radius + capsule2.radius;

	return sqDistance < r* r;
}

float Collision::sqDistanceSegmentSegment(const XMVECTOR& p1, const XMVECTOR& q1, const XMVECTOR& p2, const XMVECTOR& q2)
{
	Vec3 p1_ = { p1.m128_f32[0],p1.m128_f32[1],p1.m128_f32[2] };
	Vec3 p2_ = { p2.m128_f32[0],p2.m128_f32[1],p2.m128_f32[2] };
	Vec3 q1_ = { q1.m128_f32[0],q1.m128_f32[1],q1.m128_f32[2] };
	Vec3 q2_ = { q2.m128_f32[0],q2.m128_f32[1],q2.m128_f32[2] };

	Vec3 d1 = q1_ - p1_;//p1->q1のベクトル
	Vec3 d2 = q2_ - p2_;//p2->q2のベクトル

	Vec3 r = p1_ - p2_;

	float a = d1.dot(d1);//a = d1*d1
	float b = d1.dot(d2);//b = d1*d2
	float e = d2.dot(d2);//e = d2*d2

	float c = d1.dot(r);
	float f = d2.dot(r);

	float s = 0.0f;

	float t = 0.0f;

	float denominator = a * e - b * b;//分母

	//sの値を求める
	if (denominator != 0.0f)
	{
		s = (b * f - c * e) / denominator;
		s = clamp(s, 0.0f, 1.0f);
	}
	else//2つの線分が平行であるとき
	{
		s = 0.0f;
	}
	//tの値を求める
	//t = ((p1+d1*s)-p2)*d2/(d2*d2)
	t = (f + b * s) / e;

	//tが[0.0,1.0]の範囲外であれば、sを再計算する
	//s = ((p2+d2*t)-p1)*d1/(d1*d1) = (t*b-c)/a
	if (t < 0.0f)
	{
		t = 0.0f;
		s = clamp(-c / a, 0.0f, 1.0f);
	}
	else if (t > 1.0f)
	{
		s = clamp((b - c) / a, 0.0f, 1.0f);
		t = 1.0f;
	}
	//sとtの値が決定したので、各線分内の座標c1とc2を求める
	Vec3 c1 = p1_ + d1 * s;
	Vec3 c2 = p2_ + d2 * t;

	//2点間(c1とc2)の距離の２乗を求めて、結果を返す
	return (c1 - c2).dot(c1 - c2);
}

bool Collision::CircleCollision(const Vec2& circle1, Vec2& circle2, float radius1, float radius2)
{
	double disX = circle1.x - circle2.x;
	double disY = circle1.y - circle2.y;
	double d = disX * disX + disY * disY;
	double r = (radius1 + radius2) * (radius1 + radius2);
	return d < r;
}

bool Collision::BoxCollision(const Vec2 &box1, const Vec2 &box2, const Vec2 &size1, const Vec2 &size2)
{
	//x軸の判定
	if (box2.x - size2.x <= box1.x + size1.x && box1.x - size1.x <= box2.x + size2.x)
	{//y軸
		if (box2.y - size2.y <= box1.y + size1.y && box1.y - size1.y <= box2.y + size2.y)
		{
			return 1;
		}
	}
	return 0;
}



void OBB::Initilize(const Vec3 &pos, const Vec3 &rotation, const Vec3 &scale)
{
	//判定するOBBの情報をコピー
	m_Pos.x = pos.x;
	m_Pos.y = pos.y;
	m_Pos.z = pos.z;


	XMMATRIX rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));//Z軸まわりに４５度回転
	rotM *= XMMatrixRotationX(XMConvertToRadians(rotation.x));//X軸まわりに４５度回転
	rotM *= XMMatrixRotationY(XMConvertToRadians(rotation.y));//Y軸まわりに４５度回転

	m_NormaDirect[0].x = rotM.r[0].m128_f32[0];
	m_NormaDirect[0].y = rotM.r[0].m128_f32[1];
	m_NormaDirect[0].z = rotM.r[0].m128_f32[2];

	m_NormaDirect[1].x = rotM.r[1].m128_f32[0];
	m_NormaDirect[1].y = rotM.r[1].m128_f32[1];
	m_NormaDirect[1].z = rotM.r[1].m128_f32[2];

	m_NormaDirect[2].x = rotM.r[2].m128_f32[0];
	m_NormaDirect[2].y = rotM.r[2].m128_f32[1];
	m_NormaDirect[2].z = rotM.r[2].m128_f32[2];

	m_fLength[0] = scale.x / 2;
	m_fLength[1] = scale.y / 2;
	m_fLength[2] = scale.z / 2;
}


Vec3 OBB::GetDirect(int elem)const
{
	switch (elem)
	{
	case 0:
		return m_NormaDirect[0];
		break;
	case 1:
		return  m_NormaDirect[1];
		break;
	case 2:
		return  m_NormaDirect[2];
		break;
	default:
		return Vec3();
		break;
	}
}

float OBB::GetLen_W(int elem)const
{
	switch (elem)
	{
	case 0:
		return  m_fLength[0];
		break;
	case 1:
		return  m_fLength[1];
		break;
	case 2:
		return  m_fLength[2];
		break;
	default:
		return 0;
		break;
	}
}

//座標を取得
Vec3 OBB::GetPos_W()
{
	return m_Pos;
}

// OBB v.s. OBB
bool OBBCollision::ColOBBs(OBB& obb1, OBB& obb2)
{
	// 各方向ベクトルの確保
	Vec3 NAe1 = obb1.GetDirect(0), Ae1 = NAe1 * obb1.GetLen_W(0);
	Vec3 NAe2 = obb1.GetDirect(1), Ae2 = NAe2 * obb1.GetLen_W(1);
	Vec3 NAe3 = obb1.GetDirect(2), Ae3 = NAe3 * obb1.GetLen_W(2);
	Vec3 NBe1 = obb2.GetDirect(0), Be1 = NBe1 * obb2.GetLen_W(0);
	Vec3 NBe2 = obb2.GetDirect(1), Be2 = NBe2 * obb2.GetLen_W(1);
	Vec3 NBe3 = obb2.GetDirect(2), Be3 = NBe3 * obb2.GetLen_W(2);
	Vec3 Interval = obb1.GetPos_W() - obb2.GetPos_W();

	// 分離軸 : Ae1
	float rA = Ae1.length();
	float rB = LenSegOnSeparateAxis(NAe1, Be1, Be2, Be3);
	float L = fabsf(Interval.dot(NAe1));
	if (L > rA + rB)
		return false; // 衝突していない

	 // 分離軸 : Ae2
	rA = Ae2.length();
	rB = LenSegOnSeparateAxis(NAe2, Be1, Be2, Be3);
	L = fabsf(Interval.dot(NAe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Ae3
	rA = Ae3.length();
	rB = LenSegOnSeparateAxis(NAe3, Be1, Be2, Be3);
	L = fabsf(Interval.dot(NAe3));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be1
	rA = LenSegOnSeparateAxis(NBe1, Ae1, Ae2, Ae3);
	rB = Be1.length();
	L = fabsf(Interval.dot(NBe1));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be2
	rA = LenSegOnSeparateAxis(NBe2, Ae1, Ae2, Ae3);
	rB = Be2.length();
	L = fabsf(Interval.dot(NBe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be3
	rA = LenSegOnSeparateAxis(NBe3, Ae1, Ae2, Ae3);
	rB = Be3.length();
	L = fabsf(Interval.dot(NBe3));
	if (L > rA + rB)
		return false;

	// 分離軸 : C11
	Vec3 Cross;
	Cross = NAe1.cross(NBe1);
	rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabsf(Interval.dot(Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C12
	Cross = NAe1.cross(NBe2);
	rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabsf(Interval.dot(Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C13
	Cross = NAe1.cross(NBe3);
	rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabsf(Interval.dot(Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C21
	Cross = NAe2.cross(NBe1);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabsf(Interval.dot(Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C22
	Cross = NAe2.cross(NBe2);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabsf(Interval.dot(Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C23
	Cross = NAe2.cross(NBe3);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabsf(Interval.dot(Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C31
	Cross = NAe3.cross(NBe1);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabsf(Interval.dot(Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C32
	Cross = NAe3.cross(NBe2);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabsf(Interval.dot(Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C33
	Cross = NAe3.cross(NBe3);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabsf(Interval.dot(Cross));
	if (L > rA + rB)
		return false;

	// 分離平面が存在しないので「衝突している」
	return true;
}


// 分離軸に投影された軸成分から投影線分長を算出
float OBBCollision::LenSegOnSeparateAxis(Vec3& Sep, Vec3& e1, Vec3& e2, Vec3 e3)
{
	// 3つの内積の絶対値の和で投影線分長を計算
	// 分離軸Sepは標準化されていること
	float r1 = fabsf(Sep.dot(e1));
	float r2 = fabsf(Sep.dot(e2));
	float r3 = &e3 ? (fabsf(Sep.dot(e3))) : 0;
	return r1 + r2 + r3;
}