public class HeartEntity extends Entity {
	
	private Game game;
	
	HeartEntity (Game g, String r, int newX, int newY) {
		super(r, newX, newY);  // calls the constructor in Entity
	    game = g;
	} // constructor
	
	
	
	public void collidedWith(Entity other) {
		
		
	   } // collidedWith
	
	

}