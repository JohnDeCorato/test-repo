////////////////////////////////////////////////////////////////////////////////
//
//  IDEUM
//  Copyright 2011-2013 Ideum
//  All Rights Reserved.
//
//  Gestureworks Core
//
//  File:     TouchObject.cs
//  Authors:  Ideum
//
//  NOTICE: Ideum permits you to use, modify, and distribute this file only
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace GestureWorksTutorial3
{
    class TouchObject
    {
        public TouchObject(string name, Texture2D texture, Vector2 startPosition)
        {
            Name = name;
            Texture = texture;
            Position = startPosition;
            Origin = new Vector2(Texture.Width / 2, Texture.Height / 2);
            Scale = 1.0f;
            Rotation = 0.0f;
        }

        public string Name       { get; set; }
        public Texture2D Texture { get; set; }
        public Vector2 Position  { get; set; }
        public Vector2 Origin    { get; set; }
        public float Scale       { get; set; }
        public float Rotation    { get; set; }

        public Rectangle BoundingBox
        {
            get
            {
                int width  = (int)((float)Texture.Width  * Scale);
                int height = (int)((float)Texture.Height * Scale);

                return new Rectangle(
                    (int)Position.X - width  / 2,
                    (int)Position.Y - height / 2,
                    width,
                    height);
            }
        }

        public void Draw(SpriteBatch spriteBatch)
        {
            spriteBatch.Draw(Texture,
                             Position,
                             null,
                             Color.White,
                             Rotation,
                             Origin,
                             Scale,
                             SpriteEffects.None, 0f);
        }
    }
}
