import QtQuick 2.4

EosForm {
    function res(){
        fieldT.text="647.096";
        fieldP.text="2206400000";
        fielda.text="0.14266640249924298";
        fieldb.text ="2.112696413685528e-07";
        v1result.text = "";
        v2result.text = "";
        v3result.text = "";
    }

    function cal(){
        var T = parseFloat(fieldT.text);
        var P = parseFloat(fieldP.text);
        var a = parseFloat(fielda.text);
        var b = parseFloat(fieldb.text);

        var res = Calculator.calc(T,P,a,b);

        v1result.text = res[0];
        if( parseInt(res[3]) === 3 ){
            v2result.text = res[1];
            v3result.text = res[2];
        }
        else{
            v2result.text = " ";
            v3result.text = " ";
        }
    }

    state: {
        if( v2result.text === " ")
            return "tri";
        else
            return "";
    }
    calculate.onClicked: {
            v1result.text = "bok"
            cal()
    }
    reset.onClicked: {
            res()
    }
    transitions: Transition {
        PropertyAnimation{
            property: "opacity"
            duration: 500
        }
    }
}
