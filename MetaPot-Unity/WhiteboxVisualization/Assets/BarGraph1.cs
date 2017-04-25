using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BarGraph1 : MonoBehaviour {

	public GameObject cube1;
	public GameObject cube2;
	public GameObject cube3;
	public GameObject cube4;

	public static float Speak1 = (float)0.0;
	public static float Speak2 = (float)0.0;
	public static float Speak3 = (float)0.0;
	public static float Speak4 = (float)0.0;

	public static float TalkRate1 = (float)0.0;
	public static float TalkRate2 = (float)0.0;
	public static float TalkRate3 = (float)0.0;
	public static float TalkRate4 = (float)0.0;

	// Use this for initialization
	void Start () {
		cube1 = GameObject.Find("Cube1");
		cube2 = GameObject.Find("Cube2");
		cube3 = GameObject.Find("Cube3");
		cube4 = GameObject.Find("Cube4");
	}

	// Update is called once per frame
	void Update () {
		
		Transform tr1 = cube1.GetComponent<Transform> ();
		Transform tr2 = cube2.GetComponent<Transform> ();
		Transform tr3 = cube3.GetComponent<Transform> ();
		Transform tr4 = cube4.GetComponent<Transform> ();

		Vector3 scale1 = tr1.localScale;
		Vector3 scale2 = tr2.localScale;
		Vector3 scale3 = tr3.localScale;
		Vector3 scale4 = tr4.localScale;

		string[] parameters = UDPReceive.text.Split(',');

		if (int.Parse(parameters[0]) == 2 && int.Parse(parameters[2]) == 1)
		{
			Speak1 = float.Parse (parameters [1]);
			TalkRate1 = (Speak1 / float.Parse(parameters[3]));
			TalkRate2 = (Speak2 / float.Parse(parameters[3]));
			TalkRate3 = (Speak3 / float.Parse(parameters[3]));
			TalkRate4 = (Speak4 / float.Parse(parameters[3]));

			scale1.y = TalkRate1 * 50;
			scale2.y = TalkRate2 * 50;
			scale3.y = TalkRate3 * 50;
			scale4.y = TalkRate4 * 50;
			tr1.localScale = scale1;
			tr2.localScale = scale2;
			tr3.localScale = scale3;
			tr4.localScale = scale4;
		}

		else if (int.Parse(parameters[0]) == 2 && int.Parse(parameters[2]) == 2)
		{
			Speak2 = float.Parse (parameters [1]);
			TalkRate1 = (Speak1 / float.Parse(parameters[3]));
			TalkRate2 = (Speak2 / float.Parse(parameters[3]));
			TalkRate3 = (Speak3 / float.Parse(parameters[3]));
			TalkRate4 = (Speak4 / float.Parse(parameters[3]));

			scale1.y = TalkRate1 * 50;
			scale2.y = TalkRate2 * 50;
			scale3.y = TalkRate3 * 50;
			scale4.y = TalkRate4 * 50;
			tr1.localScale = scale1;
			tr2.localScale = scale2;
			tr3.localScale = scale3;
			tr4.localScale = scale4;
		}

		else if (int.Parse(parameters[0]) == 2 && int.Parse(parameters[2]) == 3)
		{
			Speak3 = float.Parse (parameters [1]);
			TalkRate1 = (Speak1 / float.Parse(parameters[3]));
			TalkRate2 = (Speak2 / float.Parse(parameters[3]));
			TalkRate3 = (Speak3 / float.Parse(parameters[3]));
			TalkRate4 = (Speak4 / float.Parse(parameters[3]));

			scale1.y = TalkRate1 * 50;
			scale2.y = TalkRate2 * 50;
			scale3.y = TalkRate3 * 50;
			scale4.y = TalkRate4 * 50;
			tr1.localScale = scale1;
			tr2.localScale = scale2;
			tr3.localScale = scale3;
			tr4.localScale = scale4;
		}

		else if (int.Parse(parameters[0]) == 2 && int.Parse(parameters[2]) == 4)
		{
			Speak4 = float.Parse (parameters [1]);
			TalkRate1 = (Speak1 / float.Parse(parameters[3]));
			TalkRate2 = (Speak2 / float.Parse(parameters[3]));
			TalkRate3 = (Speak3 / float.Parse(parameters[3]));
			TalkRate4 = (Speak4 / float.Parse(parameters[3]));

			scale1.y = TalkRate1 * 50;
			scale2.y = TalkRate2 * 50;
			scale3.y = TalkRate3 * 50;
			scale4.y = TalkRate4 * 50;
			tr1.localScale = scale1;
			tr2.localScale = scale2;
			tr3.localScale = scale3;
			tr4.localScale = scale4;
		}
	}

}
