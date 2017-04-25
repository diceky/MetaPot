using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BarGraph2 : MonoBehaviour {

	// Use this for initialization
	void Start () {

	}

	// Update is called once per frame
	void Update () {
		Transform tr = this.GetComponent<Transform> ();
		Vector3 scale = tr.localScale; 
		string[] parameters = UDPReceive.text.Split(',');

		if (int.Parse(parameters[0]) == 2 && int.Parse(parameters[2]) == 2)
		{
			float TalkRate = (float.Parse(parameters[1]) / float.Parse(parameters[3]));
			//SegmentLength = (Mathf.Abs(Mathf.Abs(TalkRate - (float)0.5) - (float)0.5) / (float)0.5) * 2;
			Debug.Log("Talk Rate B is " + TalkRate);
			scale.y = TalkRate * 10;
			tr.localScale = scale;
		}

	}
}
