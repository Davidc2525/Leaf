/*
* comprobar si dos tipos son iwuales
*/
function VarData(t,c){

	this.kind = 0; //var = 0 fun = 1  puntero = 2
	
	this.deph = 0;
	this.type=t||"";
	this.name="";

	this.cvd=c||null; 
};

function req(vd1,vd2){
	if(vd1.cvd!=null&&vd2.cvd!=null){
		if(vd1.type == vd2.type){

			return req(vd1.cvd,vd2.cvd)

		}else{
			return false
		}
	}else{
		if(vd1.cvd==null && vd2.cvd!=null ){
			return false
		}else if(vd2.cvd==null && vd1.cvd!=null ){
			return false
		}
		return vd1.type == vd2.type
	}
}

vd1 = new VarData("puntero",new VarData("int",null))

vd2 = new VarData("puntero", new VarData("int",null))

console.log("iwuales: "+req(vd1,vd2)+" ");
