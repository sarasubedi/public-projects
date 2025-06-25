
public class BranchLEntity extends PlatformEntity {
	
	private Game game; // game in which BranchL is created
	protected Sprite sprite; // this entity's sprite
	
	
	BranchLEntity (Game g, String r, int newX, int newY) {
		super(g, r, newX, newY);  // calls the constructor in Entity
	    game = g;
	    jumped = false; // prevents branch from moving
	    floor = y; // y value of BranchL
	} // constructor
	
	public void collidedWith(Entity other) {
	     if (other instanceof ShipEntity) {

	    	 //only sets other.y if it has landed
	    	 if(!jumped){
	    		 other.y = y - 100;
	    	 } // if
	    	 
	    	 // makes ship jump and fall if branch is touched
	    	 jump(other);
	         fall(other);
	         
	        	 if(counter == 1) {
	        		 counter = 0;
	        		 other.y -= 70;
	        		this.y -= 600;
	        		this.changeImage("sprites/branchL.png");
	        		
	        	 }else if(counter == 0) {
	        		counter++;
	        		this.changeImage("sprites/branchLT.png");
	        		
	        		
	        	 }else {
	        		 counter++;
	        	 }
	   }//if
	}//collidedWith
	
	
} // BranchLEntity