
public class BushEntity extends PlatformEntity {

	
	
private Game game;
	
	private Entity platform;
	protected Sprite sprite; // this entity's sprite
	
	
	BushEntity (Game g, String r, int newX, int newY) {
		super(g, r, newX, newY);  // calls the constructor in Entity
	    game = g;
	    jumped = false;
	    floor = y;
	} // constructor
	
	
	public void collidedWith(Entity other) {
	     if (other instanceof ShipEntity) {

	    	 //only sets other.y if it has landed
	    	 if(!jumped){
	    		 //other.y = y - 100;
	    	 }
	    	 
	    	 jump(other);
	         fall(other);
	         
	         
	         if (other instanceof ShipEntity) {
	           // remove affect entities from the Entity list
	        	 //other.changeImage("sprites/ship.png");
	        	 if(counter == 1) {
	        		 counter = 0;
	        		 other.y -= 70;
	        		this.y -= 600;
	        		this.changeImage("sprites/bush.png");
	        		
	        	 }else if(counter == 0) {
	        		counter++;
	        		this.changeImage("sprites/bushT.png");
	        		
	        		
	        	 }else {
	        		 counter++;
	        	 }


	         } // if
	         
	         
	    	 
	   }//if
	}//collidedWith
	
	
}

