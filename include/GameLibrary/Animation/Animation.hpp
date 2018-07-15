
#pragma once

#include <GameLibrary/Application/ApplicationData.hpp>
#include <GameLibrary/Utilities/ArrayList.hpp>
#include <GameLibrary/Utilities/String.hpp>
#include <GameLibrary/Utilities/Geometry/Rectangle.hpp>
#include <GameLibrary/Utilities/TypeRegistry.hpp>

namespace fgl
{
	/*! Holds Animation data for the SpriteActor class*/
	class Animation
	{
	public:
		/*! The direction the Animation should move in when animating.
			\see fgl::SpriteActor::changeAnimation(const fgl::String&,const fgl::Animation::Direction&)*/
		enum class Direction
		{
			/*! Animation direction stays the same as its previous value, which may be Direction::FORWARD, Direction::BACKWARD, or Direction::STOPPED*/
			NO_CHANGE,
			/*! frames iterate forward as time progresses*/
			FORWARD,
			/*! frames iterate backward as time progresses*/
			BACKWARD,
			/*! frames do not iterate as time progresses*/
			STOPPED
		};

		//! Represents a single frame of animation
		class Frame
		{
		public:
			Frame(TextureImage* image);
			Frame(TextureImage* image, unsigned int rows, unsigned int cols, unsigned int row, unsigned int col);

			bool operator==(const Frame& frame) const;
			bool operator!=(const Frame& frame) const;

			TextureImage* getImage() const;
			RectangleU getSourceRect() const;
			
		private:
			TextureImage* image;
			unsigned int rows;
			unsigned int cols;
			unsigned int x;
			unsigned int y;
		};

		/*! Constructs an Animation with a frame rate of 1 fps. */
		Animation();
		/*! Constructs an Animation with a specified frame rate.
			\param fps the frame rate in frames per second
			\throws fgl::IllegalArgumentException if fps is a negative value*/
		explicit Animation(float fps);
		/*! Constructs an animation with a frame rate and an array of frames
			\param fps the frame rate in frames per second
			\param frames the frames to be added
			\throws fgl::IllegalArgumentException if fps is a negative value */
		Animation(float fps, const ArrayList<Animation::Frame>& frames);
		/*! Constructs an Animation with a frame rate and a first frame.
			\param fps the frame rate in frames per second
			\param image the image to use for the first frame
			\throws fgl::IllegalArgumentException if fps is a negative value or image is null */
		Animation(float fps, TextureImage* image);
		/*! Constructs an Animation with a frame rate and frames
			\param fps the frame rate in frames per second
			\param image the image to use for the added frames
			\param rows divides the image into rows of frames. If 0, no frames are added.
			\param cols divides the image into columns of frames. If 0, no frames are added.
			\note through this function, frames are added by looping through each row and adding the columns in each row. If you want a specific order, use the sequence parameter.
			\throws fgl::IllegalArgumentException if fps is a negative value or image is null */
		Animation(float fps, TextureImage* image, unsigned int rows, unsigned int cols);
		/*! Constructs an Animation with a frame rate and frames
			\param fps the frame rate in frames per second
			\param image the image to use for the added frames
			\param rows divides the image into rows of frames. If 0, no frames are added.
			\param cols divides the image into columns of frames. If 0, no frames are added.
			\param sequence the specific sequence to order the frames. the x value of the Vector2d represents the column, and the y value represents the row; rows and columns start from 0
			\throws fgl::IllegalArgumentException if fps is a negative value or image is null */
		Animation(float fps, TextureImage* image, unsigned int rows, unsigned int cols, const ArrayList<Vector2u>& sequence);
		/*! destructor
			\note No images are unloaded when deleting. You must do that yourself.*/
		~Animation();
		
		
		/*! Checks equality of the Animation with another Animation
			\param animation the animation to compare
			\return true if equal, false if not equal */
		bool operator==(const Animation& animation) const;
		/*! Checks non-equality of the Animation with another Animation
			\param animation the animation to compare
			\return true if not equal, false if equal */
		bool operator!=(const Animation& animation) const;
		
		
		/*! Adds a single frame of animation.
			\param image the image to use for the new frame
			\throws fgl::IllegalArgumentException if the image is null */
		void addFrame(TextureImage* image);
		/*! Adds a single frame of animation.
			\param frame the animation frame to add */
		void addFrame(const Animation::Frame& frame);
		/*! Adds frames of animation.
			\param image the image to use for the added frames
			\param rows the number of rows to divide the image into
			\param cols the number of columns to divide the image into
			\throws fgl::IllegalArgumentException if the image is null, rows is 0, or cols is 0
			\note through this function, frames are added by looping through each row and adding the columns in each row. If you want a specific order, use the sequence parameter.*/
		void addFrames(TextureImage* image, unsigned int rows, unsigned int cols);
		/*! Adds frames of animation.
			\param image the image to use for the added frames
			\param rows the number of rows to divide the image into
			\param cols the number of columns to divide the image into
			\param sequence the specific sequence to order the frames. the x value of the Vector2d represents the column, and the y value represents the row
			\throws fgl::IllegalArgumentException if the image is null, rows is 0, or cols is 0 */
		void addFrames(TextureImage* image, unsigned int rows, unsigned int cols, const ArrayList<Vector2u>& sequence);
		/*! Adds frames of animation */
		void addFrames(const ArrayList<Animation::Frame>& frames);
		
		/*! Gets the frame of animation at a given index
			\param index the index of the frame to get
			\returns an Animation::Frame object */
		const Animation::Frame& getFrame(size_t index) const;
		
		/*! Removes the frame at a given index
			\param index the index of the frame to remove */
		void removeFrame(size_t index);
		/*! Removes all frames. */
		void removeAllFrames();
		
		/*! Gets the total number of frames stored in the Animation.
			\returns the total number of frames*/
		size_t getFrameCount() const;
		
		/*! Gives all the frames of the animation.
			\returns an ArrayList of AnimationFrame objects */
		const ArrayList<Animation::Frame>& getFrames() const;
		
		/*! Sets the frame rate.
			\param fps the frame rate in frames per second
			\throws fgl::IllegalArgumentException if fps is a negative value*/
		void setFPS(float fps);
		/*! Gets the frame rate.
			\returns a float representing the frame rate in frames per second*/
		float getFPS() const;
		
		/*! Sets the Animation to draw horizontally inverted.
			\param mirror true to make the Animation draw horizontally mirrored, and false to make the Animation draw normal horizontally
			\returns a pointer to this Animation object*/
		Animation* setMirroredHorizontal(bool mirror);
		/*! Sets the Animation to draw vertically inverted.
			\param mirror true to make the Animation draw vertically mirrored, and false to make the Animation draw normal vertically
			\returns a pointer to this Animation object*/
		Animation* setMirroredVertical(bool mirror);
		/*! Tells whether the Animation is mirrored horizontally.
			\see fgl::Animation::setMirroredHorizontal(bool)
			\returns true if the Animation is mirrored horizontally, false if otherwise*/
		bool isMirroredHorizontal() const;
		/*! Tells whether the Animation is mirrored vertically.
			\see fgl::Animation::setMirroredVertical(bool)
			\returns true if the Animation is mirrored vertically, false if otherwise*/
		bool isMirroredVertical() const;

		/*! Gets the frame (bounding box) of the Animation at the specified frame (animation frame), centered on (0,0)
			\param frameIndex the frame index
			\returns the bounding box of the Animation*/
		RectangleD getRect(size_t frameIndex) const;
		
		/*! Draws the specified frame centered at (0,0)
			\param graphics the graphics object used to draw the Animation
			\param frameIndex the frame number to draw */
		void drawFrame(Graphics& graphics, size_t frameIndex) const;
		/*! Draws the specified frame on the given rectangle
			\param graphics the graphics object used to draw the Animation
			\param frameIndex the frame number to draw
			\param dstRect the rectangle that the animation will be drawn within */
		void drawFrame(Graphics& graphics, size_t frameIndex, const RectangleD& dstRect) const;

	private:
		ArrayList<Animation::Frame> frames;

		float fps;

		bool mirroredHorizontal;
		bool mirroredVertical;
	};
}

REGISTER_TYPE(fgl::Animation)
