using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class text : MonoBehaviour {
    static public text instacnce;

    public UnityEngine.UI.Text UIText;
    static public Queue<string> queue = new Queue<string>(10);

    static bool isData = false;
    
    void Start () {
        instacnce = this;
	}

    static public void AddText(string str)
    {
        queue.Enqueue(str);
        isData = true;
    }

    private void Update()
    {
        if (isData)
            _AddText();
    }

    public void _AddText()
    {
        System.Text.StringBuilder sb = new System.Text.StringBuilder();

        foreach (var i in queue)
        {
            sb.AppendLine(i);
        }

        UIText.text = sb.ToString();
    }
}
